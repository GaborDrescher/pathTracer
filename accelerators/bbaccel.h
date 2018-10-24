#ifndef BB_ACCEL_H
#define BB_ACCEL_H

#include "accelerators/accelerator.h"
#include "primitives/object.h"
#include "config.h"
#include "core/aabb.h"
#include "abstraction/array.h"

	struct BVHObjectInfo
	{
		BVHObjectInfo() : object(0) { }

		BVHObjectInfo(Object *pri) : object(pri)
		{
			pri->getAABB(&bounds);
			centroid = (bounds.minC + bounds.maxC) * real(0.5);
		}

		Object *object;
		vec centroid;
		AABB bounds;
	};

    struct LinkedNode
    {
        AABB aabb;
        Array<Object*> objects;
        bool isLeaf;
        LinkedNode *left;
        LinkedNode *right;
		int axis;

        LinkedNode() : isLeaf(false), left(0), right(0), axis(0) {}
    };

	//this struct must be as compact as possible
	struct Node
	{
		AABB aabb;

		union
		{
			Object **objects;
			int secondChild;
		};

		int axis;//if axis is -1 then this node is a leaf node
		Node() : objects(0), axis(0) {}
	};

class BoxAccel : public Accelerator
{
	private:
    template<typename T>
    inline void insert(Array<T> &target, Array<T> &source) {
        for(size_t i = 0; i < source.size(); ++i) {
            target.add(source[i]);
        }
    }

	Array<Object*> *objs;
    Node *root;
	//uint32_t nBuckets;
	//uint32_t maxPrimsInNode;
	uint32_t numNodes;

	public:

	BoxAccel() : root(0), numNodes(0) {}

	void setObjects(Array<Object*> *objects)
	{
		objs = objects;
		this->init();
	}

	int makeCompactTree(const LinkedNode &node, int &index)
	{
		Node &currentNode = root[index];
		currentNode.axis = node.axis;
		currentNode.aabb = node.aabb;

		int cindex = index++;

		if(node.isLeaf)
		{
			currentNode.axis = -1;
			const int size = node.objects.size();
			currentNode.objects = new Object*[size + 1];
			for(int i = 0; i < size; ++i)
			{
				currentNode.objects[i] = node.objects[i];
			}
			currentNode.objects[size] = 0;
		}
		else
		{
			makeCompactTree(*node.left, index);
			currentNode.secondChild = makeCompactTree(*node.right, index);
		}

		return cindex;
	}

	void cleanup(LinkedNode *node)
	{
		if(!node->isLeaf)
		{
			cleanup(node->left);
			cleanup(node->right);
		}

		delete node;
	}

    void init()
	{
        //cerr << "BVH init" << endl;
        LinkedNode *node = new LinkedNode();
		Array<BVHObjectInfo> buildData;

		const size_t nobjs = objs->size();
		for(size_t i = 0; i != nobjs; ++i)
		{
			Object *current = objs->get(i);
			buildData.add(BVHObjectInfo(current));
		}

        //cerr << "BVH partition" << endl;
        numNodes = BVHPartition(*node, buildData, 0);
        //cerr << "BVH partition end" << endl;


		//posix_memalign((void**)(&root), 32, sizeof(Node) * numNodes);
		root = new Node[numNodes];

		int index = 0;
		makeCompactTree(*node, index);
		cleanup(node);
	}

	int BVHPartition(LinkedNode &node, Array<BVHObjectInfo> &buildData, int depth)
	{
		int numNodes = 1;
		AABB bbox;
		AABB cbox;


		for(size_t i = 0; i < buildData.size(); ++i)
		{
			cbox.add(buildData[i].centroid);
			bbox.add(buildData[i].bounds);
		}

		if(buildData.size() <= 3)
		{
			//leaf
			node.isLeaf = true;
			node.aabb = bbox;
			node.objects.reserve(buildData.size());
			for(size_t i = 0; i < buildData.size(); ++i)
			{
				node.objects.add(buildData[i].object);
			}
			return numNodes;
		}

		int dim = cbox.maxAxis();
		real dimcenter = (cbox.maxC[dim] + cbox.minC[dim]) * real(0.5);

		//split
		LinkedNode &left = *(new LinkedNode);
		LinkedNode &right = *(new LinkedNode);
	    Array<BVHObjectInfo> leftTri;
	    Array<BVHObjectInfo> rightTri;
		leftTri.reserve(buildData.size());
		rightTri.reserve(buildData.size());

		for(uint32_t i = 0; i < buildData.size(); ++i)
		{
			if(buildData[i].centroid[dim] > dimcenter)
			{
				right.aabb.add(buildData[i].bounds);
				rightTri.add(buildData[i]);
			}
			else
			{
				left.aabb.add(buildData[i].bounds);
				leftTri.add(buildData[i]);
			}
		}

		if((leftTri.size() == 0) || (rightTri.size() == 0))
		{
			delete &left;
			delete &right;

			//leaf
			node.isLeaf = true;
			node.aabb = bbox;
			node.objects.reserve(buildData.size());
			for(size_t i = 0; i < buildData.size(); ++i)
			{
				node.objects.add(buildData[i].object);
			}
			return numNodes;
		}

		node.left = &left;
		node.right = &right;
		node.aabb.add(left.aabb);
		node.aabb.add(right.aabb);
		node.axis = dim;

		numNodes += BVHPartition(left, leftTri, depth + 1);
		numNodes += BVHPartition(right, rightTri, depth + 1);
		return numNodes;
	}

	bool intersect(Ray *ray)
	{
		Object *obj;
		Intersection isct;
		return intersect(ray, &isct, &obj);
		/*
		Ray rayCopy = ray;
		bool hit = false;
		vec invDir(real(1) / ray.d.x, real(1) / ray.d.y, real(1) / ray.d.z);
		bool dirIsNeg[3] = {invDir.x < 0, invDir.y < 0, invDir.z < 0};

		Uint offset = 0;
		Uint nodeNum = 0;
		Uint stack[64];

		while(true)
		{
			Node &node = root[nodeNum];
			if(node.aabb.intersect(rayCopy, invDir, dirIsNeg))
			{
				if(node.axis == -1)
				{
					//intersect tris
					Object **Objects = node.Objects;

					for(int i = 0;;++i)
					{
						Object *pri = Objects[i];
						if(pri == 0)
						{
							break;
						}

						//cerr << "Object " << i << " pointer: " << tri << endl;
						if(pri->intersect(rayCopy))
						{
							return true;
						}
					}

					if(offset == 0) break;
					--offset;
					nodeNum = stack[offset];
				}
				else
				{
					if(dirIsNeg[node.axis])
					{
						stack[offset] = nodeNum + 1;
						nodeNum = node.secondChild;
					}
					else
					{
						stack[offset] = node.secondChild;
						nodeNum = nodeNum + 1;
					}
					++offset;
				}
			}
			else
			{
				if(offset == 0) break;
				--offset;
				nodeNum = stack[offset];
			}
		}

		return hit;
		*/
	}

	bool intersect(Ray *ray, Intersection *isct, Object **obj)
	{
		Ray rayCopy = *ray;
		bool hit = false;

		vec invDir(real(1) / ray->dir.x, real(1) / ray->dir.y, real(1) / ray->dir.z);
		bool dirIsNeg[3] = {invDir.x < 0, invDir.y < 0, invDir.z < 0};

		real bestT = REAL_MAX;
		Object *bestout = 0;
		Intersection bestisct;

		uint32_t offset = 0;
		uint32_t nodeNum = 0;
		uint32_t stack[64];

		while(true)
		{
			Node &node = root[nodeNum];
			if(node.aabb.intersect(rayCopy, invDir, dirIsNeg))
			{
				if(node.axis == -1)
				{
					Object **objects = node.objects;
					for(uint32_t i = 0;;++i)
					{
						Object *pri = objects[i];
						if(pri == 0)
						{
							break;
						}

						if(pri->intersect(&rayCopy, isct))
						{
							hit = true;
							if(isct->distance < bestT)
							{
								bestT = isct->distance;
								rayCopy.maxt = bestT;
								bestout = pri;
								bestisct = *isct;
							}
						}
					}

					if(offset == 0) break;
					--offset;
					nodeNum = stack[offset];
				}
				else
				{
					if(dirIsNeg[node.axis])
					{
						stack[offset] = nodeNum + 1;
						nodeNum = node.secondChild;
					}
					else
					{
						stack[offset] = node.secondChild;
						nodeNum = nodeNum + 1;
					}
					++offset;
				}
			}
			else
			{
				if(offset == 0) break;
				--offset;
				nodeNum = stack[offset];
			}
		}

		*obj = bestout;
		*isct = bestisct;

		return hit;
	}
};

#endif

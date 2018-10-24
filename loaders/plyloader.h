#ifndef PLYLOADER_H
#define PLYLOADER_H

#include <stdlib.h>
//#include <stdio.h>
#include <stdint.h>
#include "abstraction/array.h"
#include "primitives/object.h"
#include "primitives/vertex.h"
#include "primitives/triangle.h"
#include "primitives/trianglemesh.h"

class PLYLoader
{
	private:
	static const char* stof(const char *s, real *val)
	{
		s = skipWS(s);
		*val = atof(s);
		for(;;)
		{
			s++;
			if(*s == ' ' || *s == '\n') break;
		}
		return s;
	}

	static const char* stou(const char *s, uint64_t *val)
	{
		s = skipWS(s);
		*val = (uint64_t)atoi(s);
		for(;;)
		{
			s++;
			if(*s == ' ' || *s == '\n') break;
		}
		return s;
	}

	static int indexof(const char *str, const char *item)
	{
		for(size_t i = 0; str[i] != '\n'; ++i)
		{
			bool found = true;
			for(size_t k = 0; item[k] != '\0'; ++k)
			{
				if(str[i+k] == '\n')
				{
					return -1;
				}

				if(item[k] != str[i+k])
				{
					found = false;
					break;
				}
			}

			if(found)
			{
				return i;
			}
		}
		return -1;
	}

	//static size_t stringlength(const char *str, char end, size_t max)
	//{
	//	for(size_t i = 0; i != max; ++i)
	//	{
	//		if(str[i] == end) return i;
	//	}

	//	return max;
	//}

	static const char* skipWS(const char *str)
	{
		const char *out = str;
		for(;; out++)
		{
			if(*out != ' ' && *out != '\t')
			{
				break;
			}
		}

		return out;
	}

	static const char* nextLine(const char *str)
	{
		for(;;str++)
		{
			if(*str == '\0') return str;
			if(*str == '\n') return str+1;
		}
		return str+1;
	}

	public:
	//void loadfile(TriangleMesh *mesh, const char *path)
	//{
	//	FILE *f = fopen(path, "r");
	//	fseek(f, 0, SEEK_END);
	//	size_t fsize = ftell(f);
	//	fseek(f, 0, SEEK_SET);
	//	
	//	char *string = (char*)malloc(fsize + 1);
	//	size_t foo = fread(string, fsize, 1, f);
    //    (void)foo;
	//	fclose(f);
	//	
	//	string[fsize] = 0;

	//	printf("%s\n\n", path);

	//	printf("\"");
	//	for(size_t i = 0; i != fsize; ++i) {
	//		if(string[i] == '\n') {
	//			printf("\\n\"\n\"");
	//		}
	//		else {
	//			printf("%c", string[i]);
	//		}
	//	}
	//	printf("\"\n\n");


	//	load(mesh, string, fsize);
	//	free(string);
	//}
	void load(TriangleMesh *mesh, const char *file, size_t filesize)
	{
		(void)filesize;

		uint64_t vertices = 0;
		uint64_t faces = 0;

		//header
		const char *pos = file;
		int index;
		for(; *pos; pos = nextLine(pos))
		{
			if(indexof(pos, "comment") == 0)
			{
				continue;
			}

			if((index = indexof(pos, "vertex ")) >= 0)
			{
				const char *num = pos + index + 7;
				stou(num, &vertices);
				continue;
			}

			if((index = indexof(pos, "face")) >= 0)
			{
				const char *num = pos + index + 5;
				stou(num, &faces);
				continue;
			}

			if((index = indexof(pos, "end_header")) >= 0)
			{
				break;
			}

		}

		pos = nextLine(pos);

		//vertices
		mesh->vertices = new Vertex[vertices];
		mesh->nverts = vertices;
		for(uint64_t v = 0; v != vertices; ++v, pos = nextLine(pos))
		{
			//if(((v % 1000) == 0) || (v == (vertices - 1)))
			//	fprintf(stderr, "vertex: %d/%d\r", (int)v+1, (int)vertices);
			pos = skipWS(pos);

			vec vert;
			pos = skipWS(stof(pos, &vert.x));
			pos = skipWS(stof(pos, &vert.y));
			pos = skipWS(stof(pos, &vert.z));


			mesh->vertices[v].pos = vert;
		}
		//fprintf(stderr, "\n");

		//faces
		mesh->triangles = new Triangle[faces];
		mesh->ntris = faces;
		for(uint64_t f = 0; f != faces; ++f, pos = nextLine(pos))
		{
			//if(((f % 1000) == 0) || (f == (faces - 1)))
			//	fprintf(stderr, "triangle: %d/%d\r", (int)f+1, (int)faces);
			pos = skipWS(pos);

			uint64_t idx;
			pos = skipWS(stou(pos, &idx));
			//number of indices
	

			pos = skipWS(stou(pos, &idx));
			mesh->triangles[f].a = &(mesh->vertices[idx]);

			pos = skipWS(stou(pos, &idx));
			mesh->triangles[f].b = &(mesh->vertices[idx]);

			pos = skipWS(stou(pos, &idx));
			mesh->triangles[f].c = &(mesh->vertices[idx]);
		}
		//fprintf(stderr, "\n");
	}
};


#endif

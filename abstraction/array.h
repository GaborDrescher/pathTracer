#ifndef ARRAY_H
#define ARRAY_H

#include <stdlib.h>
#include "abstraction/types.h"
#include "abstraction/memory.h"

template<typename T>
class ArrayData
{
	public:
	T *data;
};

template<typename T>
class Array
{
	private:
	T *data;
	size_t cap;
	size_t msize;

	public:
	Array()
	{
		data = 0;
		cap = 0;
		msize = 0;
	}
    ~Array()
    {
		free(data);
    }

	void add(T item)
	{
		if(msize == cap)
		{
            if(cap == 0) {
                cap = 1;
            }
			cap *= 2;
			T *tmp = (T*)realloc(data, sizeof(T) * cap);
			if(tmp == 0) {
				abort();
			}
			data = tmp;
		}

		data[msize] = item;
		msize += 1;
	}

	void enlarge(int s)
	{
		cap = s;
		T *tmp = (T*)realloc(data, sizeof(T) * cap);
		if(tmp == 0) {
			abort();
		}
		data = tmp;
	}
	void reserve(int s)
	{
        enlarge(s);
	}

	size_t getsize() const
	{
		return size();
	}

	size_t size() const
	{
		return msize;
	}

	T get(size_t idx) const
	{
		return data[idx];
	}

	T operator[](size_t idx) const
	{
		return get(idx);
	}
};

#endif

#pragma once
#include <string>
#include "DataContainers.h"
#include "MemoryCache.h"
#include "Vector.h"
namespace Tungsten {
struct MemoryBlock
{
	void* ptr;
	int width;
	MemoryBlock()
	{
		ptr=0;
		width=0;
	}
	MemoryBlock(void* ptr,int width)
	{
		this->ptr=ptr;
		this->width=width;
	}
	static MemoryBlock Null()
	{
		return MemoryBlock();
	}
};


template <typename _val,typename _tag> class Pair
{
public:
	_tag tag;
	_val val;
public:
	Pair(_val val,_tag tag)
	{
		this->tag=tag;
		this->val=val;
	}
	Pair()
	{

	}
};

template <typename _val,typename _tag=std::wstring> class Dictionary;
template <typename _val,typename _tag> class Dictionary
{
	unsigned int lastpos;
private:
	typedef Pair<_val,_tag> _pair;
	MemoryCache< _pair > data;
public:
	Dictionary()
	{
		lastpos=0;
	}
	void Register(_pair pair)
	{
		data.Expand(lastpos+1);
		*(data.GetPtr()+lastpos)=pair;
		lastpos++;

	}
	void Register(_val val, _tag tag)
	{
		_pair aux(val,tag);
		data.Expand(lastpos+1);
		*(data.GetPtr()+lastpos)=aux;
		lastpos++;
	}
	_val* Seek    (_tag tag)
	{
		for(unsigned int i=0;i<lastpos;i++)
		{
			if( (data.GetPtr()+i)->tag==tag) return &((data.GetPtr()+i)->val);
		}
		return NULL;
	}
	_pair* operator [](unsigned int index)
	{
		if(index>=lastpos) return NULL;
		return data.GetPtr()+index;
	}

};



template<typename T> class DynamicArray
	{
	private:
		MemoryCache<T> memory;
	public:
		T& operator[](int index)
		{
			return *(memory.GetPtr()+index);
		};
	};
}

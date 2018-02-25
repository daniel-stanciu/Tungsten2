#pragma once
#define _SCL_SECURE_NO_WARNINGS
#include <memory>
template<class Ty>
class MemoryCache
{
private:
	//Pointer to the allocated area of memory
	Ty* ptr;
	int iterator;
	int elem_capacity;
	float scaleFactor;
public:
	
	MemoryCache(int minSize=0,float scaleFactor=1.5f)
	{
		ptr=NULL;
		this->elem_capacity=minSize;
		this->scaleFactor=scaleFactor;
		ptr=new Ty[elem_capacity];
		iterator=0;
	};
	//Sets the capacity to at least the elem_count argument, while clearing all the data
	void ResetCapacity(int element_count)
	{
		if(element_count<minSize) element_count=minSize;
		if(element_count>elem_capacity)
		{
			if(elem_capacity==elem_capacity*scaleFactor) elem_capacity=element_count;
			while(elem_capacity<element_count)
			{
				elem_capacity=(int)(elem_capacity*scaleFactor);
			}
			delete[] ptr;
			ptr=new Ty[elem_capacity];
		}
		if(element_count<elem_capacity)
		{
			elem_capacity=element_count;
			
			delete[] ptr;
			ptr=new Ty[elem_capacity];
		}
	}
	//Expands the capacity, keeping the content intact
	void Expand(int element_count)
	{
		
		if(element_count>elem_capacity)
		{
			int copy_size=elem_capacity;
			if(copy_size==(int)(elem_capacity*scaleFactor) ) elem_capacity=element_count;
			while(elem_capacity<element_count)
			{
				elem_capacity=(int)(elem_capacity*scaleFactor);
			}
			Ty* aux=new Ty[elem_capacity];

			for(int i=0;i<copy_size;i++)
			{
				aux[i]=ptr[i];
			}
			//UNSAFE
			//std::copy(ptr,ptr+elem_capacity,aux);
			//Does not work with ref_counted classes
			//memcpy(aux,ptr,copy_size*sizeof(Ty));
			delete[] ptr;
			ptr=aux;
		}

	}
	Ty* GetPtr()
	{
		return ptr;
	}
	~MemoryCache(void)
	{
		delete[] ptr;
	}
	void PushBack(Ty element)
	{
		Expand(++iterator);
		ptr[iterator]=element;
	}
	int IteratorPos()
	{
		return iterator;
	}
	void Reset()
	{
		iterator=0;
		ResetCapacity(minSize);
	}
};



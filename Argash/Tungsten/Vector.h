#include <pch.h>
namespace Tungsten
{


template<unsigned int dimensions,typename T=long> class Vector
{
private:
	T data[dimensions];
public:
	T& operator[](int index)
		{
			return ((T*)(this))[index];
		};
	template<typename T2>
	Vector<dimensions,T2>	operator=(Vector<dimensions,T2> arg)
	{
		for(int i=0;i<dimensions;i++)
		{
			this->data[i]=(T)arg[i];
		}
		return this;
	};
	Vector<dimensions,T>	operator+(Vector<dimensions,T> arg)
	{
		Vector<dimensions,T> ret;
		for(int i=0;i<dimensions;i++)
		{
			ret[i]=this->operator[](i)+(T)arg[i];
		}
		return ret;
	}
};

template<typename T> class Vector<2,T>
{
public:
	T x;
	T y;
	T& operator[](int index)
		{
			return ((T*)(this))[index];
		};
	
	Vector(){};
	Vector(T x,T y)
	{
		this->x=x;
		this->y=y;
	}
	template<typename T2>
	Vector<2,T>& operator=(Vector<2,T2> arg)
	{
		for(int i=0;i<2;i++)
		{
			this->operator[](i)=(T)arg[i];
		}
		return *this;
	};
	Vector<2,T>	operator+(Vector<2,T> arg)
	{
		Vector<2,T> ret;
		for(int i=0;i<2;i++)
		{
			ret[i]=this->operator[](i)+(T)arg[i];
		}
		return ret;
	}
};

template<typename T> class Vector<3,T>
{
public:
	T x;
	T y;
	T z;
	T& operator[](int index)
		{
			return ((T*)(this))[index];
		};
	Vector(){};
	Vector(T x, T y,T z)
	{
		this->x=x;
		this->y=y;
		this->z=z;
	}
	template<typename T2>
	Vector<3,T2> operator=(Vector<3,T2> arg)
	{
		for(int i=0;i<2;i++)
		{
			this->operator[](i)=(T)arg[i];
		}
		return this;
	};
	Vector<3,T>	operator+(Vector<3,T> arg)
	{
		Vector<3,T> ret;
		for(int i=0;i<3;i++)
		{
			ret[i]=this->operator[](i)+(T)arg[i];
		}
		return ret;
	}
};

template<typename T> class Vector<4,T>
{
public:
	T x;
	T y;
	T z;
	T w;
	T& operator[](int index)
		{
			return ((T*)(this))[index];
		};
	Vector(){};
	Vector(T x, T y,T z,T w)
	{
		this->x=x;
		this->y=y;
		this->z=z;
		this->w=w;
	}
	template<typename T2>
	Vector<4,T>& operator=(Vector<4,T2> arg)
	{
		for(int i=0;i<2;i++)
		{
			this->operator[](i)=(T)arg[i];
		}
		return this;
	};
	Vector<4,T>	operator+(Vector<4,T> arg)
	{
		Vector<4,T> ret;
		for(int i=0;i<4;i++)
		{
			ret[i]=this->operator[](i)+(T)arg[i];
		}
		return ret;
	}
};
}
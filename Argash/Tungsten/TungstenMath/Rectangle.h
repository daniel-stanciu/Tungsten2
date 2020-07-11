#pragma once
#include "Vector.h"
namespace Tungsten
{
	template<typename T>
	struct RectanglePoints
	{
		Point<T> p[4];
		Point<T>& operator[](int i)
		{
			return p[i];
		}
	};
	template<typename T> class Rectangle
	{
	public:
		Point<T> Center;
		Point<T> Size;

		Rectangle(Point<T> center, Point<T> size)
		{
			Center = center;
			Size = size;
		}
		Rectangle()
		{
			Center = { 0,0 };
			Size = { 0,0 };
		}



		template<typename T>
		static Rectangle<T> FromTopLeftSize(Point<T> topLeft, Point<T> size)
		{
			Point<T> center = { topLeft.x+size.x/2,topLeft.y+size.y/2 };
			
			Rectangle<T> aux = { {center.x,center.y},{size.x,size.y}};
			return aux;
		}
		static Rectangle FromTopLeftBottomRight(Point<T> topLeft, Point<T> bottomRight)
		{
			auto size = { bottomRight.x - topLeft.x,bottomRight.y - topLeft.y };
			return TopLeftSize(topLeft, size);
		}
		Point<T> TopLeft() {
			return Center - Size / 2 ;
		}
		Point<T> BottomRight()
		{
			return { Center + Size / 2 };
		}
		Point<T> BottomLeft() {
			return { Center.x - Size.x / 2,Center.y + Size.y / 2 };
		}
		Point<T> TopRight() {
			return { Center.x + Size.x / 2,Center.y - Size.y / 2 };
		}

		RectanglePoints<T> ClockWise() {
			RectanglePoints<T> pts;
			pts[0] = TopLeft();
			pts[1] = TopRight();
			pts[2] = BottomLeft();
			pts[3] = BottomRight();
			return pts;
		}

		RectanglePoints<T> CounterClockWise() {
			RectanglePoints<T> pts;
			pts[0] = TopLeft();
			pts[1] = BottomLeft();
			pts[2] = TopRight();
			pts[3] = BottomRight();

			return pts;
		}

	};
}

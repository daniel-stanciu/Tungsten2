#include "PixelCoordinates.h"

Pixels operator""_px(unsigned long long int arg)
{
	return (Pixels)arg;
}


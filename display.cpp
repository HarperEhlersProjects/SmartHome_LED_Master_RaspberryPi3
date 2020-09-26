#include "display.h"

#include <cstdlib>

//Allocating memory for display matrix
void DPU::allocateMatrix()
{
	this->matrix = (tsRGB**)malloc(this->resolution.y * sizeof(tsRGB*));

	for (uint8_t i = 0; i < this->resolution.y; i++)
	{
		matrix[i] = (tsRGB*)malloc(this->resolution.y * sizeof(tsRGB));
	}
}

//Reset display matrix
void DPU::resetMatrix()
{
	uint8_t x, y;

	for (y = 0; y < this->resolution.y; y++)
	{
		for (x = 0; x < this->resolution.x; x++)
		{
			matrix[y][x] = { 0, 0, 0 };
		}
	}
}

//Whether a point is inside the display or not
bool DPU::isInsideBorders(tsCoord coord)
{
	return (coord.x >= 0 && coord.x < this->resolution.x&& coord.y >= 0 && coord.y < this->resolution.y);
}
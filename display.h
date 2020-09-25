#ifndef DISPLAY_H
#define DISPLAY_H

#pragma once

#include "color.h"

#include <cstdint>

typedef struct {
	uint8_t x;
	uint8_t y;
} tsResolution;


class DPU
{
public:
	tsRGB** matrix;
	tsResolution resolution;

	DPU()
	{
		this->resolution = { 16,16 };
		allocateMatrix();
	}

	DPU(tsResolution resolution)
	{
		this->resolution = resolution;
		allocateMatrix();
	}

	~DPU()
	{
		for (uint8_t i = 0; i < this->resolution.y; i++)
		{
			delete matrix[i];
		}

		delete matrix;
	}

	void resetMatrix(void);
	void allocateMatrix(void);
};



#endif // !DISPLAY_H





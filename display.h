#ifndef DISPLAY_H
#define DISPLAY_H

#pragma once

#include "color.h"

#include <cstdint>

typedef struct {
	uint8_t x;
	uint8_t y;
} tsResolution,tsCoord;


class DPU
{
public:
	tsRGB** matrix;
	tsResolution resolution;

	DPU();
	DPU(tsResolution resolution);

	~DPU();

	void resetMatrix(void);
	void allocateMatrix(void);

	bool isInsideBorders(tsCoord coord);
};



#endif // !DISPLAY_H





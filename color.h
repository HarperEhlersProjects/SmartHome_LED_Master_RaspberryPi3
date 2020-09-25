#ifndef COLOR_H
#define COLOR_H

#pragma once

#include <cstdint>

//Structure that defines pixelcolor in RGB format.
typedef struct {
	uint8_t uiRed;
	uint8_t uiGreen;
	uint8_t uiBlue;
} tsRGB;

//Structure that defines pixelcolor in HSV format.
typedef struct {
	long uiHuel;
	double udSaturation;
	double udBrightness;
} tsHSV;


//convert color from HSV to RGB format.
tsRGB tsHSV2RGB(tsHSV);

#endif // !COLOR_H
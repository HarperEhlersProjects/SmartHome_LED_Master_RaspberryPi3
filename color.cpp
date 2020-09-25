#include "color.h"


//Using formular from wikipedia "HSV-Farbraum"
tsRGB tsHSV2RGB(tsHSV tsColorHSV)
{
	tsRGB tsColorTempRGB;

	char h;
	double f, p, q, t, correction;

	h = (char)(tsColorHSV.uiHuel / 60.0);
	f = (tsColorHSV.uiHuel / 60.0 - h);

	p = tsColorHSV.udBrightness * (1 - tsColorHSV.udSaturation);
	q = tsColorHSV.udBrightness * (1 - tsColorHSV.udSaturation * f);
	t = tsColorHSV.udBrightness * (1 - tsColorHSV.udSaturation * (1 - f));

	switch (h)
	{
	case 0: tsColorTempRGB.uiRed = (char)(tsColorHSV.udBrightness * 255);
		tsColorTempRGB.uiGreen = (char)(t * 255);
		tsColorTempRGB.uiBlue = (char)(p * 255);
		break;
	case 1: tsColorTempRGB.uiRed = (char)(q * 255);
		tsColorTempRGB.uiGreen = (char)(tsColorHSV.udBrightness * 255);
		tsColorTempRGB.uiBlue = (char)(p * 255);
		break;
	case 2: tsColorTempRGB.uiRed = (char)(p * 255);
		tsColorTempRGB.uiGreen = (char)(tsColorHSV.udBrightness * 255);
		tsColorTempRGB.uiBlue = (char)(t * 255);
		break;
	case 3: tsColorTempRGB.uiRed = (char)(p * 255);
		tsColorTempRGB.uiGreen = (char)(q * 255);
		tsColorTempRGB.uiBlue = (char)(tsColorHSV.udBrightness * 255);
		break;
	case 4: tsColorTempRGB.uiRed = (char)(t * 255);
		tsColorTempRGB.uiGreen = (char)(p * 255);
		tsColorTempRGB.uiBlue = (char)(tsColorHSV.udBrightness * 255);
		break;
	case 5: tsColorTempRGB.uiRed = (char)(tsColorHSV.udBrightness * 255);
		tsColorTempRGB.uiGreen = (char)(p * 255);
		tsColorTempRGB.uiBlue = (char)(q * 255);
		break;
	case 6: tsColorTempRGB.uiRed = (char)(tsColorHSV.udBrightness * 255);
		tsColorTempRGB.uiGreen = (char)(t * 255);
		tsColorTempRGB.uiBlue = (char)(p * 255);
	}

	return tsColorTempRGB;
}

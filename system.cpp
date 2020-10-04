#include "system.h"


VirtualSLA::VirtualSLA()
{
	char i;


	this->mode.number = MODES_MODE_BLANK;
	for (i = 0; i < MODE_PARAMETER_NUMBER; i++)
	{
		this->mode.parameter[i] = 0;
	}
	for (i = 0; i < MODE_ACTOR_NUMBER; i++)
	{
		this->mode.actors[i] = 0;
	}

	setNumberOfSegments(VIRTUAL_SLA_SEGMENTS_NUMBER);
	setFrameBufferSize(0);
}

void VirtualSLA::setFrameBufferSize(long size)
{
	this->framebuffer.setSize(size);
}

long VirtualSLA::getFrameBufferSize()
{
	return this->framebuffer.size();
}

void VirtualSLA::setNumberOfSegments(long number)
{
	char i, previousNumberOfSegments = this->segments.size();

	this->numberOfSegments = number;

	if (this->numberOfSegments != previousNumberOfSegments)
	{
		this->segments.resize(this->numberOfSegments);

		for (i = previousNumberOfSegments; i < this->segments.size(); i++)
		{
			this->segments[i] = { VIRTUAL_SLA_DEST_NONE,FALSE,0,0,0 };
		}
	}
}

void VirtualSLA::serializeDPUMatrix(void)
{
	int x, y, index;

	for (x = 0; x < this->display.resolution.x; x++)
	{
		for (y = 0; y < this->display.resolution.y; y += 2)
		{
			index = y * this->display.resolution.x + (this->display.resolution.x - 1) - x;

			this->framebuffer.vSetPixel(index, (tsRGB) this->display.matrix[y][x]);
		}

		for (y = 1; y < this->display.resolution.y; y += 2)
		{
			index = y * this->display.resolution.x + x;

			this->framebuffer.vSetPixel(index, (tsRGB) this->display.matrix[y][x]);
		}
	}
}

System::System()
{
	SLALengthMax = SLA_LENGTH_MAX;

	setNumberOfvSLAs(VIRTUAL_SLA_NUMBER);
}

void System::setNumberOfvSLAs(long number)
{
	this->numberOfvSLAs = number;

	if (this->numberOfvSLAs != virtualSLAs.size())
	{
		while (this->numberOfvSLAs < virtualSLAs.size())
		{
			delete virtualSLAs[virtualSLAs.size() - 1];
			virtualSLAs.pop_back();
		}

		while (this->numberOfvSLAs > virtualSLAs.size())
		{
			virtualSLAs.push_back(new VirtualSLA());
		}
	}
}
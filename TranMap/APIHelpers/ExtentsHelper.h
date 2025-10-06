#pragma once
#include "Extents.h"
class ExtentsHelper
{
public:
	static CExtents* Populate(Extent& extent);
	static CExtents* GetWorldBounds();
};


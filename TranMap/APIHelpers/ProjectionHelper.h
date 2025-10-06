#pragma once
#include "GeoProjection.h"
class ProjectionHelper
{
public:
	static bool IsEmpty(CGeoProjection* gp);
	static bool IsGeographic(CGeoProjection* gp);
	static bool IsSame(CGeoProjection* gp1, CGeoProjection* gp2, CExtents* bounds, int sampleSize);
};


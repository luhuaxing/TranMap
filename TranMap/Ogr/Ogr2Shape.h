#pragma once
#include "ogr_feature.h"
#include "OgrHelper.h"

class Ogr2Shape
{
public:
	static CShapefile* CreateShapefile(OGRLayer* layer);
	static CShapefile* Layer2Shapefile(OGRLayer* layer, int maxFeatureCount, bool& isTrimmed, OgrDynamicLoader* loader, ICallback* callback = NULL);
	static bool FillShapefile(OGRLayer* layer, CShapefile* sf, int maxFeatureCount, bool loadLabels, ICallback* callback, bool& isTrimmed);
};


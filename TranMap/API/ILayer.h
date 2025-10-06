#pragma once
enum eLayerType
{
	NULL_LAYER = 4,
	SHAPFILE_LAYER = 0,
	IMAGE_LAYER    = 1,
	GRID_LAYER     = 2,
	OGR_LAYER      = 3
};
class ILayer
{
public:
	virtual eLayerType GetLayerType()=0;
	virtual ~ILayer() {}
};
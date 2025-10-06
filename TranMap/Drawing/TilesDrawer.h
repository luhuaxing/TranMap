#pragma once
#include "basedrawer.h"
#include "Tiles.h"

class CTilesDrawer : public CBaseDrawer
{
public:
	// old version for GDI drawing
	CTilesDrawer(CDC* dc, Extent* extents, double pixelPerProjectionX, double pixelPerProjectionY)
	{
		_dc = dc;
		_extents = extents;
		_pixelPerProjectionX = pixelPerProjectionX;
		_pixelPerProjectionY = pixelPerProjectionY;
		m_transfomation = NULL;
		m_graphics = NULL;
	};
	// newer one for GDI+ drawing
	CTilesDrawer(Gdiplus::Graphics* g, Extent* extents, double pixelPerProjectionX, double pixelPerProjectionY)
	{
		_dc = NULL;
		m_graphics = g;
		_extents = extents;
		_pixelPerProjectionX = pixelPerProjectionX;
		_pixelPerProjectionY = pixelPerProjectionY;
		m_transfomation = NULL;
	};
	~CTilesDrawer(void){};

	void DrawTiles(CTiles* tiles, double pixelsPerDegree, CGeoProjection* mapProjection, BaseProjection* tileProjection, bool printing, int projectionChangeCount);

	CGeoProjection* m_transfomation;
	Gdiplus::Graphics* m_graphics;
};

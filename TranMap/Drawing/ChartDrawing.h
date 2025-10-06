#pragma once
#include "BaseDrawer.h"
#include "CollisionList.h"

class CChartDrawer: public CBaseDrawer
{
public:
	// constructor
	CChartDrawer(Gdiplus::Graphics* graphics, Extent* extents, double dx, double dy, double currentScale, CCollisionList* collisionList)
	{
		_dc = NULL;
		_extents = extents;
		_pixelPerProjectionX = dx;
		_pixelPerProjectionY = dy;
		_collisionList = collisionList;
		_currentScale = currentScale;
		_graphics = graphics;
	}
	
	~CChartDrawer(void){}

protected:	
	double _currentScale;
	CCollisionList* _collisionList;

private:
	bool WithinVisibleExtents(double xMin, double xMax, double yMin, double yMax)
	{
		return 	
			!(xMin > _extents->right || xMax < _extents->left || yMin > _extents->top || yMax < _extents->bottom);
	};

public:
	void DrawCharts(CShapefile* sf);
};



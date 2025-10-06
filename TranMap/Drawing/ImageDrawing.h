
#pragma once
#include "BaseDrawer.h"
#include "ImageGroup.h"

class CImageDrawer: public CBaseDrawer
{
public:
	// constructor
	CImageDrawer::CImageDrawer(Gdiplus::Graphics* graphics, Extent* extents, double dx, double dy, long viewWidth, long viewHeight)
	{
		_extents = extents;
		_pixelPerProjectionX = dx;
		_pixelPerProjectionY = dy;
		_viewHeight = viewHeight;
		_viewWidth = viewWidth;
		_dc = NULL;
		_graphics = graphics;
	}
	
	// destructor
	CImageDrawer::~CImageDrawer() {}
	
	// member variables
protected:	
	Extent _visibleExtents;
	CRect _visibleRect;

	long _viewWidth;
	long _viewHeight;

private:
	bool CImageDrawer::CalculateImageBlockSize(int& dstL, int& dstT, int& dstR, int& dstB, 
								   int& imgX, int& imgY, int& imgW, int& imgH, 
						           double& mapL, double& mapT, double& mapR, double& mapB,
								   const double dx, const double dy, const CRect& rcBounds);

	bool CImageDrawer::CalculateImageBlockSize1(Extent extents, double MinX, double MinY, double MaxX, double MaxY,
										   const double dX, const double dY, const int height, const int width);

	bool WithinVisibleExtents(double xMin, double xMax, double yMin, double yMax)
	{
		return 	
			!(xMin > _extents->right || xMax < _extents->left || yMin > _extents->top || yMax < _extents->bottom);
	};

public:
	ScreenBitmap* CImageDrawer::DrawImage(const CRect & rcBounds, CImageClass* iimg, bool returnBitmap = false);	

};


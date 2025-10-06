#include "stdafx.h"
#include "BaseProjection.h"

// gets matrix size in decimal degrees
void BaseProjection::GetTileSizeLatLon(PointLatLng point, int zoom, SizeLatLng &ret)
{
	CPoint temp;
	this->FromLatLngToXY(point, zoom, temp);
	this->GetTileSizeLatLon(temp, zoom, ret);
}

// gets matrix size in decimal degrees
void BaseProjection::GetTileSizeLatLon(CPoint point, int zoom, SizeLatLng &ret)
{
	PointLatLng pnt1;
	this->FromXYToLatLng(point, zoom, pnt1);

	// moving to the neighbouring tile
	CPoint newPoint(point.x, point.y);

	CSize size;
	GetTileMatrixMaxXY(zoom, size);
	if (newPoint.x < size.cx)	newPoint.x++;
	else						newPoint.x--;

	if (newPoint.y < size.cy)	newPoint.y++;
	else						newPoint.y--;

	PointLatLng pnt2;
	this->FromXYToLatLng(newPoint, zoom, pnt2);
    
	// size
	ret.WidthLng = fabs( pnt2.Lng - pnt1.Lng);
	ret.HeightLat = fabs(pnt2.Lat - pnt1.Lat);
}

// gets matrix size in tile coordinates
void BaseProjection::GetTileMatrixSizeXY(int zoom, CSize &ret)
{
	CSize sMin, sMax;
	GetTileMatrixMinXY(zoom, sMin);
	GetTileMatrixMaxXY(zoom, sMax);
	ret.cx = sMax.cx - sMin.cx + 1;
	ret.cy = sMax.cy - sMin.cy + 1;
}
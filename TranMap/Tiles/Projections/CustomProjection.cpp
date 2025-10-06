#include "stdafx.h"
#include "CustomProjection.h"

// Converts from decimal degrees to tile coordinates
void CustomProjection::FromLatLngToXY(PointLatLng pnt, int zoom, CPoint& ret)
{
	double lat = pnt.Lat;
	double lng = pnt.Lng;
	
	VARIANT_BOOL vb;
	projWGS84->Transform(&lng, &lat, &vb);
	
	this->FromProjToXY(lat, lng, zoom, ret);
}

// Converts from projected units to tile coordinates
void CustomProjection::FromProjToXY(double lat, double lng, int zoom, CPoint &ret)
{
	lat = Clip(lat, yMinLat, yMaxLat);
	lng = Clip(lng, xMinLng, xMaxLng);

	double y = (lat - yMinLat)/(yMaxLat - yMinLat);
	double x = (lng - xMinLng)/(xMaxLng - xMinLng);
	
	CSize s;
	GetTileMatrixSizeXY(zoom, s);
	int mapSizeX = s.cx;
	int mapSizeY = s.cy;

	ret.x = (int) Clip(x * mapSizeX, 0, mapSizeX);
	ret.y = (int) Clip(y * mapSizeY, 0, mapSizeY);

	Clip(ret, zoom);
}

// Converts tile coordinates to decimal degrees
void CustomProjection::FromXYToLatLng(CPoint pnt, int zoom, PointLatLng &ret)
{
	FromXYToProj(pnt, zoom, ret);

	VARIANT_BOOL vb;
	projCustom->Transform(&ret.Lng, &ret.Lat, &vb);

	ret.Lat = ret.Lat;
	ret.Lng = ret.Lng;
}

// Converts from tile coordinates to projected coordinates
void CustomProjection::FromXYToProj(CPoint pnt, int zoom, PointLatLng &ret)
{
	CSize s;
	GetTileMatrixSizeXY(zoom, s);
	double mapSizeX = s.cx;
	double mapSizeY = s.cy;

	double x = Clip(pnt.x, 0, mapSizeX) / mapSizeX;
	double y = Clip(pnt.y, 0, mapSizeY) / mapSizeY;

	x = xMinLng + x * (xMaxLng - xMinLng);
	y = yMinLat + y * (yMaxLat - yMinLat);

	ret.Lat = y;
	ret.Lng = x;
}

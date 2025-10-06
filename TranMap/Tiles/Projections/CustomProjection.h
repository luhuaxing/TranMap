#pragma once
#include "BaseProjection.h"

// A base class for projected coordinte systems, i.e. specified in meters rather than decimal degrees
// Additional transformation with proj4.dll is carries out for them
class CustomProjection: public BaseProjection
{
protected:	
	CGeoProjection* projWGS84;
public:
	CGeoProjection* projCustom;
	
	CustomProjection()
	{
		projected = true;
		projWGS84 = new CGeoProjection();
		projCustom = new CGeoProjection();
	};

	virtual ~CustomProjection()
	{
		projWGS84->StopTransform();
		projCustom->StopTransform();
		delete projCustom;
		delete projWGS84;
	}

	void FromLatLngToXY(PointLatLng pnt, int zoom, CPoint &ret);
	void FromProjToXY(double lat, double lng, int zoom, CPoint &ret);
	void FromXYToLatLng(CPoint pnt, int zoom, PointLatLng &ret);
	void FromXYToProj(CPoint pnt, int zoom, PointLatLng &ret);

	double GetWidth()
	{
		return xMaxLng - xMinLng;
	}

	void GetTileSizeProj(int zoom, CSize &size)
	{
		this->GetTileMatrixSizeXY(zoom, size);
		size.cx = (long)((xMaxLng - xMinLng)/(double)size.cx);
		size.cy = (long)((yMaxLat - yMinLat)/(double)size.cy);
	}
};
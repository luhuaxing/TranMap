 
#pragma once
#include "BaseProvider.h"

class WMSGMercProvider: public BaseProvider
{
CString url2;
public:
	~WMSGMercProvider(void){};

	WMSGMercProvider() 
	{
		this->Projection = new MercatorProjection();
	}
	// gets bounding box in Google mercator projection (meters; EPSG:3857)
	CString GetBoundingBox(CPoint &pos, int zoom)
	{
		PointLatLng pnt1;
		((MercatorProjection*)this->Projection)->TileXYToProj(pos, zoom, pnt1);

		PointLatLng pnt2;
		pos.x++;
		pos.y++;
		((MercatorProjection*)this->Projection)->TileXYToProj(pos, zoom, pnt2);
		
		CString s;
		s.Format("%f,%f,%f,%f", pnt1.Lng, pnt2.Lat, pnt2.Lng, pnt1.Lat);
		return s;
	}
};


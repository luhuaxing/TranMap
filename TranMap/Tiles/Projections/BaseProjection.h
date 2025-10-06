
#pragma once
#include "GlobalVars.h"
#include "geopoint.h"

// A base abstract class to handle transformation between map coordinates (decimal degrees)
// and tile coordinates (indices of tiles at particular zoom level)
class BaseProjection
{
protected:
	double MinLatitude;
	double MaxLatitude;
	double MinLongitude;
	double MaxLongitude;
public:
    CSize tileSize;
	double yMinLat;		// in decimal degrees for Mercator projection or projected units for custom projections
	double yMaxLat;
	double xMinLng;
	double xMaxLng;
	bool yInverse;
	double PI;
	bool projected;	  // it's projected coordinate system; direct calculations of tile positions will be attempted
	double earthRadius;
	bool worldWide;
	tkTileProjection serverProjection;

	double GetMinLatitude() { 
		return projected ? MinLatitude : yMinLat; 
	}
	double GetMaxLatitude() { 
		return projected ? MaxLatitude : yMaxLat; }
	double GetMinLongitude() { 
		return projected ? MinLongitude : xMinLng; 
	}
	double GetMaxLongitude() { 
		return projected ? MaxLongitude : xMaxLng; 
	}

	virtual void FromLatLngToXY(PointLatLng pnt, int zoom, CPoint &ret) = 0;
	virtual void FromXYToLatLng(CPoint pnt, int zoom, PointLatLng &ret) = 0;
	virtual double GetWidth() = 0;

	BaseProjection()
	{
		 tileSize = CSize(256, 256);
		 PI = 3.1415926535897932384626433832795;
		 yInverse = false;
		 earthRadius = 6378137.0;
		 worldWide = true;
		 serverProjection = tkTileProjection::SphericalMercator;
		 yMinLat = yMaxLat = xMinLng = xMaxLng = 0.0;
		 projected = false;
	};
	
	virtual ~BaseProjection() {}

	void GetTileMatrixMinXY(int zoom, CSize &size)
	{
		size.cx = 0;
		size.cy = 0;
	}

	void GetTileMatrixMaxXY(int zoom, CSize &size)
	{
		int xy = (1 << zoom);
		size.cx = xy-1;
		size.cy = xy-1;
	}

	static double Clip(double n, double minValue, double maxValue)
	{
	   return MIN(MAX(n, minValue), maxValue);
    }

	void Clip(CPoint& tilePnt, int zoom)
	{
		CSize size; 
		GetTileMatrixMaxXY(zoom, size);
		tilePnt.x = MIN(MAX(tilePnt.x, 0), size.cx);
		tilePnt.y = MIN(MAX(tilePnt.y, 0), size.cy);
    }

	void GetTileSizeLatLon(PointLatLng point, int zoom, SizeLatLng &ret);
	void GetTileSizeLatLon(CPoint point, int zoom, SizeLatLng &ret);
	void GetTileMatrixSizeXY(int zoom, CSize &ret);

	RectLatLng CalculateGeogBounds(CPoint pnt, int zoom)
	{
		// calculating geographic coordinates
		SizeLatLng size;
		this->GetTileSizeLatLon(pnt, zoom, size );

		if (size.WidthLng == 0.0 || size.HeightLat == 0.0) {
			CallbackHelper::AssertionFailed("Invalid tile size on calculating geographic bounds.");
		}

		if (this->yInverse)
		{
			PointLatLng geoPnt;
			CPoint pnt2 = pnt;
			pnt2.y++;			// y corresponds to the bottom of tile as the axis is directed up
			// while the drawing position is defined by its top-left corner
			// so the calculation is made by the upper tile

			this->FromXYToLatLng(pnt2, zoom, geoPnt);
			return RectLatLng(geoPnt.Lng, geoPnt.Lat, size.WidthLng, size.HeightLat);
		}
		else
		{
			PointLatLng geoPnt;
			this->FromXYToLatLng(pnt, zoom, geoPnt);
			return RectLatLng(geoPnt.Lng, geoPnt.Lat, size.WidthLng, size.HeightLat );
		}
	}
};

class MercatorBase: public BaseProjection
{
	double GetWidth()
	{
		//return MERCATOR_MAX_VAL * 2.0;
		return 20037508.342789244*2.0;
	}
};


 
#pragma once
#include "BaseProvider.h"
#include "MercatorProjection.h"

class BingBaseProvider: public BaseProvider
{
protected:
	CString _imagerySet;
public:
	BingBaseProvider() 
	{
		RefererUrl = "http://www.bing.com/maps/";
		Copyright = L"Copyright ?2014 Microsoft and its suppliers";
		this->Projection = new MercatorProjection();
		subProviders.push_back(this);
	}
	
	bool Initialize();
	CString TileXYToQuadKey(int tileX, int tileY, int levelOfDetail);

	CString MakeTileImageUrl(CPoint &pos, int zoom)
	{
		// http://ecn.{subdomain}.tiles.virtualearth.net/tiles/r{quadkey}.jpeg?g=3179&mkt={culture}&shading=hill
		CString key = TileXYToQuadKey(pos.x, pos.y, zoom);
		CString subDomain;
		subDomain.Format("t%d", GetServerNum(pos, 4));

		CString temp = UrlFormat;
		temp.Replace("{quadkey}", key);
		temp.Replace("{culture}", LanguageStr);
		temp.Replace("{subdomain}", subDomain);

		return temp;
	}

	virtual CStringW GetCopyright()
	{
		if (UrlFormat.GetLength() == 0) {
			return L"INVALID BING MAPS API KEY";
		}
		else {
			return Copyright;
		}
	}
};

class BingMapProvider: public BingBaseProvider
{
public:
	BingMapProvider() 
	{
		_imagerySet = "Road";
		Id = tkTileProvider::BingMaps;
		Name = "BingMaps";
	}
};

class BingSatelliteProvider: public BingBaseProvider
{
public:
	BingSatelliteProvider() 
	{
		_imagerySet = "Aerial";
		Id = tkTileProvider::BingSatellite;
		Name = "BingSatellite";
	}
};

class BingHybridProvider: public BingBaseProvider
{
public:
	BingHybridProvider() 
	{
		_imagerySet = "AerialWithLabels";
		Id = tkTileProvider::BingHybrid;
		Name = "BingHybrid";
	}
};


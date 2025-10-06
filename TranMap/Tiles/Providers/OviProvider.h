 
#pragma once
#include "BaseProvider.h"

class OviBaseProvider: public BaseProvider
{
private:	
	CString UrlServerLetters;
public:

	OviBaseProvider() 
	{
		RefererUrl = "https://www.here.com/";
        int year = Utility::GetCurrentYear();
		Copyright.Format(L"?d Here, DigitalGlobe", year);
		UrlServerLetters = "1234";
		this->Projection = new MercatorProjection();
		subProviders.push_back(this);
	}
	
	CString MakeTileImageUrl(CPoint &pos, int zoom)
	{
		CString s;
		s.Format(UrlFormat, UrlServerLetters[GetServerNum(pos, 4)], zoom, pos.x, pos.y);
		s.Format("%s?app_id=%s&app_code=%s", s, m_globalSettings.hereAppId, m_globalSettings.hereAppCode);
		return s;
	}

	bool Initialize() 
	{
		if (m_globalSettings.hereAppCode.GetLength() == 0 ||
			m_globalSettings.hereAppId.GetLength() == 0) {
			CallbackHelper::ErrorMsg("Application ID or (and) code are not set for Here Maps tiles provider. See GlobalSettings.SetHereMapsApiKey.");
			return false;
		}
		return true;
	}

	virtual CStringW GetCopyright()
	{
		if (m_globalSettings.hereAppCode.GetLength() == 0 ||
			m_globalSettings.hereAppId.GetLength() == 0) {
			return L"INVALID HERE MAPS APP_ID";
		}
		else {
			return Copyright;
		}
	}
};

class OviMapProvider: public OviBaseProvider
{
public:
	OviMapProvider() 
	{
		Id = tkTileProvider::HereMaps;
		Name = "Here Maps";
		UrlFormat = "http://%c.base.maps.api.here.com/maptile/2.1/maptile/newest/normal.day/%d/%d/%d/256/png8";
	}
};

class OviSatelliteProvider: public OviBaseProvider
{
public:
	OviSatelliteProvider() 
	{
		Id = tkTileProvider::HereSatellite;
		Name = "Here Satellite";
		UrlFormat = "http://%c.aerial.maps.api.here.com/maptile/2.1/maptile/newest/satellite.day/%d/%d/%d/256/png8";
	}
};

class OviHybridProvider: public OviBaseProvider
{
public:
	OviHybridProvider() 
	{
		Id = tkTileProvider::HereHybrid;
		Name = "Here Hybrid";
		UrlFormat = "http://%c.aerial.maps.api.here.com/maptile/2.1/maptile/newest/hybrid.day/%d/%d/%d/256/png8";
	}
};

class OviTerrainProvider: public OviBaseProvider
{
public:
	OviTerrainProvider() 
	{
		Id = tkTileProvider::HereTerrain;
		Name = "Here Terrain";
		UrlFormat = "http://%c.aerial.maps.api.here.com/maptile/2.1/maptile/newest/terrain.day/%d/%d/%d/256/png8";
	}
};


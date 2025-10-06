 
#pragma once
#include "BaseProvider.h"
#include "WMSGMercProvider.h"

class RosreestrProvider: public BaseProvider
{
public:
	RosreestrProvider(bool labels) 
	{
		Id = tkTileProvider::Rosreestr;
		Name = "Rosreestr";
		Copyright = "©Росреестр";
		ServerLetters = "abc";
		LicenseUrl = "http://maps.rosreestr.ru/PortalOnline/terms.html";
		RefererUrl = "http://maps.rosreestr.ru/";
		CString type = labels ? "Anno" : "BaseMap";
		UrlFormat = "http://%c.maps.rosreestr.ru/arcgis/rest/services/BaseMaps/" + type + "/MapServer/tile/%d/%d/%d";
		this->Projection = new MercatorProjection();
		this->maxZoom = 19;
	}
	CString MakeTileImageUrl(CPoint &pos, int zoom)
	{
		char letter = ServerLetters[GetServerNum(pos, 3)];
		CString s; 
		s.Format(UrlFormat, letter, zoom,  pos.y, pos.x);
		return s;
	}
};

class RosreestrBordersProvider: public WMSGMercProvider
{
private:	
	CString url2;
public:
	RosreestrBordersProvider() 
	{
		Id = tkTileProvider::Rosreestr;
		Name = "Rosreestr";
		Copyright = "";
		LicenseUrl = "http://maps.rosreestr.ru/PortalOnline/terms.html";
		RefererUrl = "http://maps.rosreestr.ru/";
		UrlFormat = "http://maps.rosreestr.ru/arcgis/rest/services/Cadastre/Cadastre/MapServer/export?dpi=96&transparent=true&format=png32&bbox=";
		url2 = "&bboxSR=102100&imageSR=102100&size=256%2C256&f=image";
		this->subProviders.push_back(this);
	}

	CString MakeTileImageUrl(CPoint &pos, int zoom)
	{
		CString s = UrlFormat + GetBoundingBox(pos, zoom) + url2; 
		return s;
	}
};
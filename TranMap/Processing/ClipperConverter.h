#pragma once
#include "clipper.h"

class ClipperConverter
{
public:	
	// coordinates for clipper are multiplied in case of geographic coordinate systems
	double conversionFactor;
	ClipperConverter() : conversionFactor(1.0){	}
	ClipperConverter(CShapefile* sf) {
		this->SetConversionFactor(sf);
	}
	void SetConversionFactor(CShapefile* sf);
	ClipperLib::Polygons* Shape2ClipperPolygon(CShape* shp);
	CShape* ClipperPolygon2Shape(ClipperLib::Polygons* polygon);
	static CShape* ClipPolygon(CShape* shapeClip, CShape* shapeSubject, PolygonOperation operation);
	static void AddPolygons(CShapefile* sf, ClipperLib::Clipper& clp, ClipperLib::PolyType clipType, bool selectedOnly);
	static ClipperLib::Polygons* ClipPolygon(ClipperLib::Polygons* polyClip, ClipperLib::Polygons* polySubject, ClipperLib::ClipType operation);
};
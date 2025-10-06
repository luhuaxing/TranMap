#pragma once
#include "Gdiplus.h"

// -------------------------------------------------------
// CLineSegment interface
// -------------------------------------------------------
class  CLineSegment
{
public:
	
	CLineSegment()
	{
		_lineType = lltSimple;
		_lineColor = RGB(0,0,0);
		_markerOutlineColor = RGB(120, 120, 120);
		_lineWidth = 1.0f;
		_lineStyle = dsSolid;
		_marker = dpsDiamond;
		_markerSize = 12;
		_markerInterval = 16;
		_markerOffset = 0.0f;
		_markerOrientation = lorParallel;
		_markerFlipFirst = VARIANT_TRUE;
		gReferenceCounter.AddRef(idLineSegment);
	}

	~CLineSegment()
	{
		gReferenceCounter.Release(idLineSegment);
	}

public:
	void get_LineType(tkLineType* retVal);
	void put_LineType(tkLineType newVal);
	void get_Color(OLE_COLOR* retVal);
	void put_Color(OLE_COLOR newVal);
	void get_LineWidth(float* retVal);
	void put_LineWidth(float newVal);
	void get_LineStyle(tkDashStyle* retVal);
	void put_LineStyle(tkDashStyle newVal);
	// marker line
	void get_Marker(tkDefaultPointSymbol* retVal);
	void put_Marker(tkDefaultPointSymbol newVal);
	void get_MarkerSize(float* retVal);
	void put_MarkerSize(float newVal);
	void get_MarkerInterval(float* retVal);
	void put_MarkerInterval(float newVal);
	void get_MarkerOrientation(tkLineLabelOrientation* retVal);
	void put_MarkerOrientation(tkLineLabelOrientation newVal);
	void get_MarkerFlipFirst(VARIANT_BOOL* retVal);
	void put_MarkerFlipFirst(VARIANT_BOOL newVal);
	void get_MarkerOffset(float* retVal);
	void put_MarkerOffset(float newVal);
	void get_MarkerOutlineColor(OLE_COLOR* retVal);
	void put_MarkerOutlineColor(OLE_COLOR newVal);
	void Draw(int** hdc, float x, float y, int clipWidth, int clipHeight, OLE_COLOR backColor, VARIANT_BOOL* retVal);
	void DrawVB(int hdc, float x, float y, int clipWidth, int clipHeight, OLE_COLOR backColor, VARIANT_BOOL* retVal);
	void Draw(Gdiplus::Graphics& g, BYTE transparency, int ImageWidth, int ImageHeight, int xOrigin, int yOrigin, VARIANT_BOOL* retVal);

private:
	tkLineType _lineType;
	OLE_COLOR _lineColor;
	float _lineWidth;
	tkDashStyle _lineStyle;
	tkDefaultPointSymbol _marker;
	float _markerSize;
	float _markerInterval;
	float _markerOffset;
	OLE_COLOR _markerOutlineColor;
	tkLineLabelOrientation _markerOrientation;
	VARIANT_BOOL _markerFlipFirst;

public:
	VARIANT_BOOL DrawCore(CDC* dc, float x, float y, int clipWidth, int clipHeight, OLE_COLOR backColor);

};
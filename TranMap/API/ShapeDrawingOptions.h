#pragma once
#include "DrawingOptions.h"
#include "Image.h"
#include "LinePattern.h"


// **************************************************************
//    CShapeDrawingOptions
// **************************************************************
class  CShapeDrawingOptions 
{
public:
	CShapeDrawingOptions()
	{	
		_lastErrorCode = tkNO_ERROR;
		_isLineDecoration = false;
		gReferenceCounter.AddRef(tkInterface::idShapeDrawingOptions);
	}
	~CShapeDrawingOptions()
	{			
		gReferenceCounter.Release(tkInterface::idShapeDrawingOptions);
	}


public:
	void get_Visible(VARIANT_BOOL *pVal);
	void put_Visible(VARIANT_BOOL newVal);
	// drawing flags
	void get_FillVisible(VARIANT_BOOL *pVal);
	void put_FillVisible(VARIANT_BOOL newVal);
	void get_LineVisible(VARIANT_BOOL *pVal);
	void put_LineVisible(VARIANT_BOOL newVal);
	// colors	
	void get_FillColor(OLE_COLOR *pVal);
	void put_FillColor(OLE_COLOR newVal);
	void get_LineColor(OLE_COLOR *pVal);
	void put_LineColor(OLE_COLOR newVal);
	
	void get_DrawingMode(tkVectorDrawingMode *pVal);
	void put_DrawingMode(tkVectorDrawingMode newVal);

	// stipples
	void get_FillHatchStyle(tkGDIPlusHatchStyle *pVal);
	void put_FillHatchStyle(tkGDIPlusHatchStyle newVal);
	void get_LineStipple(tkDashStyle *pVal);
	void put_LineStipple(tkDashStyle newVal);
	void get_PointShape(tkPointShapeType *pVal);
	void put_PointShape(tkPointShapeType newVal);

	// size, width, transparency
	void get_FillTransparency(float *pVal);
	void put_FillTransparency(float newVal);
	void get_LineWidth(float *pVal);
	void put_LineWidth(float newVal);
	void get_PointSize(float *pVal);
	void put_PointSize(float newVal);

	// stipple transparency
	void get_FillBgTransparent(VARIANT_BOOL* pVal);
	void put_FillBgTransparent(VARIANT_BOOL newVal);
	void get_FillBgColor(OLE_COLOR* pVal);
	void put_FillBgColor(OLE_COLOR newVal);
	
	// raster fill
	void get_Picture(CImageClass** pVal);
	void put_Picture(CImageClass* newVal);

	void get_FillType(tkFillType* pVal);
	void put_FillType(tkFillType newVal);					
	void get_FillGradientType(tkGradientType* pVal);
	void put_FillGradientType(tkGradientType newVal);
	void get_PointType(tkPointSymbolType* pVal);
	void put_PointType(tkPointSymbolType newVal);			
	void get_FillColor2(OLE_COLOR *pVal);
	void put_FillColor2(OLE_COLOR newVal);
	void get_PointRotation(double *pVal);
	void put_PointRotation(double newVal);

	void get_PointSidesCount(long *pVal);
	void put_PointSidesCount(long newVal);
	
	void get_PointSidesRatio(float *pVal);
	void put_PointSidesRatio(float newVal);

	void get_FillRotation(double *pVal);
	void put_FillRotation(double newVal);
	
	void get_FillGradientBounds(tkGradientBounds *pVal);
	void put_FillGradientBounds(tkGradientBounds newVal);

	void get_LineTransparency(float *pVal);
	void put_LineTransparency(float newVal);

	void get_PictureScaleX(double *pVal);
	void put_PictureScaleX(double newVal);
	void get_PictureScaleY(double *pVal);
	void put_PictureScaleY(double newVal);

	void get_AlignPictureByBottom(VARIANT_BOOL *pVal);
	void put_AlignPictureByBottom(VARIANT_BOOL newVal);
	
	// .NET overloads
	void DrawPoint(int** hdc, float x, float y, 
						 int clipWidth, int clipHeight, OLE_COLOR backColor, 
						 VARIANT_BOOL* retVal);

	void DrawLine(int** hdc, float x, float y, int width, int height, VARIANT_BOOL drawVertices, 
						int clipWidth, int clipHeight, OLE_COLOR backColor, 
						VARIANT_BOOL* retVal);

	void DrawRectangle(int** hdc, float x, float y, int width, int height, VARIANT_BOOL drawVertices, 
						int clipWidth, int clipHeight, OLE_COLOR backColor, 
						VARIANT_BOOL* retVal);

	void DrawShape(int** hdc, float x, float y, CShape* shape,  VARIANT_BOOL drawVertices,
						int clipWidth, int clipHeight, OLE_COLOR backColor , 
						VARIANT_BOOL* retVal);

	// VB6 overloads
	void DrawPointVB(int hdc, float x, float y, 
						 int clipWidth, int clipHeight, OLE_COLOR backColor, 
						 VARIANT_BOOL* retVal);

	void DrawLineVB(int hdc, float x, float y, int width, int height, VARIANT_BOOL drawVertices, 
						int clipWidth, int clipHeight, OLE_COLOR backColor, 
						VARIANT_BOOL* retVal);

	void DrawRectangleVB(int hdc, float x, float y, int width, int height, VARIANT_BOOL drawVertices, 
						int clipWidth, int clipHeight, OLE_COLOR backColor, 
						VARIANT_BOOL* retVal);

	void DrawShapeVB(int hdc, float x, float y, CShape* shape,  VARIANT_BOOL drawVertices,
						int clipWidth, int clipHeight, OLE_COLOR backColor, 
						VARIANT_BOOL* retVal);

	void get_PointCharacter(short* retVal);
	void put_PointCharacter(short newVal);

	void get_FontName(BSTR* retval);		
	void put_FontName(BSTR newVal);					

	void Clone(CShapeDrawingOptions** retval);

	void get_ErrorMsg(/*[in]*/ long ErrorCode, /*[out, retval]*/ BSTR *pVal);
	void get_LastErrorCode(/*[out, retval]*/ long *pVal);

	void get_VerticesVisible(VARIANT_BOOL* retval);
	void put_VerticesVisible(VARIANT_BOOL newVal);
	void get_VerticesSize(LONG* retval);
	void put_VerticesSize(LONG newVal);
	void get_VerticesColor(OLE_COLOR* retval);
	void put_VerticesColor(OLE_COLOR newVal);
	void get_VerticesFillVisible(VARIANT_BOOL* retval);
	void put_VerticesFillVisible(VARIANT_BOOL newVal);
	void get_VerticesType(tkVertexType* retval);
	void put_VerticesType(tkVertexType newVal);

	void get_FrameVisible(VARIANT_BOOL* retval);
	void put_FrameVisible(VARIANT_BOOL newVal);

	void get_FrameType(tkLabelFrameType* retval);
	void put_FrameType(tkLabelFrameType newVal);

	void get_LinePattern(CLinePattern** retVal);
	void put_LinePattern(CLinePattern* newVal);
	
	void get_Tag(BSTR* retVal);		
	void put_Tag(BSTR newVal);					

	void SetGradientFill(OLE_COLOR color, short range);
	void SetDefaultPointSymbol(tkDefaultPointSymbol symbol);

	void get_UseLinePattern(VARIANT_BOOL* retVal);		
	void put_UseLinePattern(VARIANT_BOOL newVal);		

	void Serialize(BSTR* retVal);
	void Deserialize(BSTR newVal);

	void get_MinScale(double *pVal);
	void put_MinScale(double newVal);

	void get_MinLineWidth(double *pVal);
	void put_MinLineWidth(double newVal);

	void get_MaxScale(double *pVal);
	void put_MaxScale(double newVal);

	void get_MaxLineWidth(double *pVal);
	void put_MaxLineWidth(double newVal);

	void get_DynamicVisibility(VARIANT_BOOL* pVal);
	void put_DynamicVisibility(VARIANT_BOOL newVal);
	void get_MinVisibleScale(DOUBLE* pVal);
	void put_MinVisibleScale(DOUBLE newVal);
	void get_MaxVisibleScale(DOUBLE* pVal);
	void put_MaxVisibleScale(DOUBLE newVal);
	
private:
	CDrawingOptionsEx _options;
	long _lastErrorCode;
	bool _isLineDecoration;  // prevents the creation of child instances of CDrawingOptions class in case it's line decoration symbol, 
							 // which could become endless loop
private:
	VARIANT_BOOL DrawPointCore(CDC* dc, float x, float y, 
							   int clipWidth = 0, int clipHeight = 0, OLE_COLOR backColor = 16777215);

	VARIANT_BOOL DrawLineCore(CDC* dc, float x, float y, int width, int height, VARIANT_BOOL drawVertices,
							  int clipWidth = 0, int clipHeight = 0, OLE_COLOR backColor = 16777215);

	VARIANT_BOOL DrawRectangleCore(CDC* dc, float x, float y, int width, int height, VARIANT_BOOL drawVertices,
							  int clipWidth = 0, int clipHeight = 0, OLE_COLOR backColor = 16777215);

	VARIANT_BOOL DrawShapeCore(CDC* dc, float x, float y, CShape* shape, VARIANT_BOOL drawVertices, 
							  int clipWidth = 0, int clipHeight = 0, OLE_COLOR backColor = 16777215);

public:	
	bool DeserializeCore(CPLXMLNode* node);
	CPLXMLNode* SerializeCore(CString ElementName);

	void ErrorMessage(long ErrorCode) {	_lastErrorCode = ErrorCode;	}

	CDrawingOptionsEx* get_UnderlyingOptions() {return &_options; }
	void put_underlyingOptions(CDrawingOptionsEx* newVal) {	_options = *newVal; }
	void put_IsLineDecoration(bool newVal){	_isLineDecoration = newVal;	}
	void get_IsLineDecoration(bool* retVal)	{*retVal = _isLineDecoration;}
};
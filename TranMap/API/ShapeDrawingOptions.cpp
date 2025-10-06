#include "stdafx.h"
#include "ShapeDrawingOptions.h"
#include "ImageAttributes.h"
#include "macros.h"
#include "PointSymbols.h"
#include "Image.h"
#include "LinePattern.h"
#include "Shape.h"

// *************************************************************
//		get_UseLinePattern
// *************************************************************
void  CShapeDrawingOptions::get_UseLinePattern(VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if (!_options.CanUseLinePattern() && _options.useLinePattern)
	{
		_options.useLinePattern = false;
	}
	*retVal = _options.useLinePattern ? VARIANT_TRUE : VARIANT_FALSE;
	return ;
}

// *************************************************************
//		put_UseLinePattern
// *************************************************************
void  CShapeDrawingOptions::put_UseLinePattern(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	_options.useLinePattern = newVal && _options.CanUseLinePattern();
	return ;
}

// *************************************************************
//		get_LastErrorCode
// *************************************************************
void  CShapeDrawingOptions::get_LastErrorCode(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	*pVal = _lastErrorCode;
	_lastErrorCode = tkNO_ERROR;

	return ;
}

// *************************************************************
//		get_ErrorMsg
// *************************************************************
void  CShapeDrawingOptions::get_ErrorMsg(long ErrorCode, BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	USES_CONVERSION;
	*pVal = A2BSTR(ErrorMsg(ErrorCode));

	return ;
}

// *******************************************************
//	   get_Picture()
// *******************************************************
void  CShapeDrawingOptions::get_Picture(CImageClass** pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*pVal = _options.picture;
	return ;
};

// *******************************************************
//	   put_Picture()
// *******************************************************
void  CShapeDrawingOptions::put_Picture(CImageClass* newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if (!newVal) {
		//ComHelper::SetRef(newVal, (IDispatch**)&_options.picture);
		return ;
	}

	tkImageSourceType type;
	newVal->get_SourceType(&type);

	long width, height;
	newVal->get_Width(&width);
	newVal->get_Height(&height);
	if (width * height > 3000000)
	{
		ErrorMessage(tkICON_OR_TEXTURE_TOO_BIG);
	}
	else
	{
		if (type == istGDALBased)
		{
			if (width == 0 || height == 0)
			{
				VARIANT_BOOL vbretval;
				newVal->LoadBuffer(10, &vbretval); 
			}
		}
		
		newVal->get_Width(&width);
		newVal->get_Height(&height);
		
		if (width > 0 && height > 0)
		{
			;
			//ComHelper::SetRef(newVal, (IDispatch**)&_options.picture);
		}
	}
	return ;
};

// There are 2 overloads for each function: 
// hdc passed as int** - for new Graphics handle
// hdc passed as int - for VB6/VBA 

#pragma region DrawPoint
// *******************************************************
//	   DrawPoint()
// *******************************************************
void  CShapeDrawingOptions::DrawPoint(int** hdc, float x, float y, int clipWidth, int clipHeight, OLE_COLOR backColor, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	CDC* dc = CDC::FromHandle((HDC)hdc);
	
	if (clipWidth == 0)
		clipWidth = (int)_options.pointSize + 1;
	
	if (clipHeight == 0)
		clipHeight =  (int)_options.pointSize + 1;

	*retVal = DrawPointCore(dc, x, y, clipWidth, clipHeight, backColor);
	return ;
}

// *******************************************************
//	   DrawPointVB()
// *******************************************************
void  CShapeDrawingOptions::DrawPointVB(int hdc, float x, float y, int clipWidth, int clipHeight, OLE_COLOR backColor, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	CDC* dc = CDC::FromHandle((HDC)hdc);
	
	if (clipWidth == 0)
		clipWidth = (int)_options.pointSize + 1;
	
	if (clipHeight == 0)
		clipHeight =  (int)_options.pointSize + 1;

	*retVal = DrawPointCore(dc, x, y, clipWidth, clipHeight, backColor);
	return ;
}

// *******************************************************
//	   DrawPointCore()
// *******************************************************
VARIANT_BOOL CShapeDrawingOptions::DrawPointCore(CDC* dc, float x, float y, int clipWidth, int clipHeight, OLE_COLOR backColor )
{
	if (!dc)
	{
		ErrorMessage(tkFAILED_TO_OBTAIN_DC);
		return VARIANT_FALSE;
	}
	
	CShape* shp = new CShape();
	if (!shp) 
		return VARIANT_FALSE;

	VARIANT_BOOL vbretval;
	shp->Create(SHP_POINT, &vbretval);
	if (!vbretval)
	{
		delete shp;
		return VARIANT_FALSE;
	}

	CPointClass* pnt = new CPointClass();
	pnt->put_X(clipWidth/2.0); 
	pnt->put_Y(clipHeight/2.0);
	long position = 0;
	shp->InsertPoint(pnt, &position, &vbretval); 
	delete pnt;
	
	VARIANT_BOOL result = this->DrawShapeCore(dc, x, y, shp, VARIANT_FALSE, clipWidth, clipHeight, backColor);
	delete shp;

	return result;
}
#pragma endregion

#pragma region DrawLine

// *******************************************************
//	   DrawLine()
// *******************************************************
void  CShapeDrawingOptions::DrawLine(int** hdc, float x, float y, int width, int height, VARIANT_BOOL drawVertices, int clipWidth, int clipHeight, OLE_COLOR backColor, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	CDC* dc = CDC::FromHandle((HDC)hdc);

	if (clipWidth == 0)
		clipWidth = width + 1;

	if (clipHeight == 0)
		clipHeight = height + 1;

	*retVal = DrawLineCore(dc, x, y, width, height, drawVertices, clipWidth, clipHeight, backColor);
	return ;
}

// *******************************************************
//	   DrawLineVB()
// *******************************************************
void  CShapeDrawingOptions::DrawLineVB(int hdc, float x, float y, int width, int height, VARIANT_BOOL drawVertices, int clipWidth, int clipHeight, OLE_COLOR backColor, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	CDC* dc = CDC::FromHandle((HDC)hdc);
	
	if (clipWidth == 0)
		clipWidth = width + 1;

	if (clipHeight == 0)
		clipHeight = height + 1;

	*retVal = DrawLineCore(dc, x, y, width, height, drawVertices, clipWidth, clipHeight, backColor);
	return ;
}

// *******************************************************
//	   DrawLineCore()
// *******************************************************
VARIANT_BOOL CShapeDrawingOptions::DrawLineCore(CDC* dc, float x, float y, int width, int height, VARIANT_BOOL drawVertices, int clipWidth, int clipHeight, OLE_COLOR backColor)
{
	if (!dc)
	{
		ErrorMessage(tkFAILED_TO_OBTAIN_DC);
		return VARIANT_FALSE;
	}
	
	CShape* shp = new CShape();
	if (!shp) return VARIANT_FALSE;

	VARIANT_BOOL vbretval;
	shp->Create(SHP_POLYLINE, &vbretval);
	
    long position = 0;
    shp->InsertPart(0, &position, &vbretval);
    
	CPointClass* pnt = NULL;
	
	for (int i = 0; i < 2; i++)
	{
		pnt = new CPointClass();
		
		if ( i == 0 )
		{
			if (_options.verticesVisible && drawVertices)
			{
				pnt->put_X(_options.verticesSize/2); 
				pnt->put_Y(height/2);
			}
			else
			{
				pnt->put_X(0.0); 
				pnt->put_Y(height/2);
			}
		}
		else if ( i == 1 )
		{
			if (_options.verticesVisible && drawVertices)
			{
				pnt->put_X(width); 
				pnt->put_Y(height/2);
			}
			else
			{
				pnt->put_X(width - _options.verticesSize/2); 
				pnt->put_Y(height/2);
			}
		}
		shp->InsertPoint(pnt, &position, &vbretval); position++;
		delete pnt;
	}
	
	VARIANT_BOOL retVal = this->DrawShapeCore(dc, x, y, shp, drawVertices, clipWidth, clipHeight, backColor);
	delete shp;
	return retVal;
}
#pragma endregion

#pragma region DrawRectangle

// *****************************************************************
//	   DrawRectangle()
// *****************************************************************
// We shall create the rectangle shape here
void  CShapeDrawingOptions::DrawRectangle(int** hdc, float x, float y, int width, int height, VARIANT_BOOL drawVertices, 
												 int clipWidth, int clipHeight, OLE_COLOR backColor, VARIANT_BOOL* retVal )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	CDC* dc = CDC::FromHandle((HDC)hdc);

	if (clipWidth == 0)
		clipWidth = width + 1;

	if (clipHeight == 0)
		clipHeight = height + 1;

	*retVal = this->DrawRectangleCore(dc, x, y, width, height, drawVertices, clipWidth, clipHeight, backColor);
	return ;
}

// *****************************************************************
//	   DrawRectangleVB()
// *****************************************************************
void  CShapeDrawingOptions::DrawRectangleVB(int hdc, float x, float y, int width, int height, VARIANT_BOOL drawVertices, 
												   int clipWidth, int clipHeight, OLE_COLOR backColor, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CDC* dc = CDC::FromHandle((HDC)hdc);
	
	if (clipWidth == 0)
		clipWidth = width + 1;

	if (clipHeight == 0)
		clipHeight = height + 1;

	*retVal = DrawRectangleCore(dc, x, y, width, height, drawVertices, clipWidth, clipHeight, backColor);
	return ;
}

// ****************************************************************
//		DrawRectangleCore()
// ****************************************************************
VARIANT_BOOL CShapeDrawingOptions::DrawRectangleCore(CDC* dc, float x, float y, int width, int height, VARIANT_BOOL drawVertices, int clipWidth, int clipHeight, OLE_COLOR backColor )
{
	if (!dc)
	{
		ErrorMessage(tkFAILED_TO_OBTAIN_DC);
		return VARIANT_FALSE;
	}
	
	CShape* shp = new CShape();
	if (!shp) return VARIANT_FALSE;

	VARIANT_BOOL vbretval;
	shp->Create(SHP_POLYGON, &vbretval);
	
    long position = 0;
    shp->InsertPart(0, &position, &vbretval);
    
	CPointClass* pnt = NULL;
	
	for (int i = 0; i <= 4; i++)	// <=4
	{
		pnt = new CPointClass();
		
		if (i == 0 || i == 4)
		{
			if (_options.verticesVisible && drawVertices)
			{
				pnt->put_X(_options.verticesSize/2); 
				pnt->put_Y(_options.verticesSize/2);
			}
			else
			{
				pnt->put_X(0.0); 
				pnt->put_Y(0.0);
			}
		}
		else if (i == 1)
		{
			if (_options.verticesVisible && drawVertices)
			{
				pnt->put_X(width - _options.verticesSize/2); 
				pnt->put_Y(_options.verticesSize/2);
			}
			else
			{
				pnt->put_X(width); 
				pnt->put_Y(0.0);
			}
		}
		else if (i == 2)
		{
			if (_options.verticesVisible && drawVertices)
			{
				pnt->put_X(width - _options.verticesSize/2); 
				pnt->put_Y(height - _options.verticesSize/2); 
			}
			else
			{
				pnt->put_X(width); 
				pnt->put_Y(height);
			}
		}
		else if (i == 3)
		{
			if (_options.verticesVisible && drawVertices)
			{
				pnt->put_X(_options.verticesSize/2); 
				pnt->put_Y(height - _options.verticesSize/2); 
			}
			else
			{
				pnt->put_X(0.0); 
				pnt->put_Y(height);
			}
		}

		shp->InsertPoint(pnt, &position, &vbretval); position++;
		delete pnt;
	}
	
	VARIANT_BOOL retVal = this->DrawShapeCore(dc, x, y, shp, drawVertices, clipWidth, clipHeight, backColor);
	delete shp;
	return retVal;

	return VARIANT_TRUE;
}
#pragma endregion

#pragma region DrawShape

// *****************************************************************
//	   DrawShape()
// *****************************************************************
// We shall create the rectangle shape here
void  CShapeDrawingOptions::DrawShape(int** hdc, float x, float y, CShape* shape, VARIANT_BOOL drawVertices, 
											 int clipWidth, int clipHeight, OLE_COLOR backColor, 
											 VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	CDC* dc = CDC::FromHandle((HDC)hdc);

	*retVal = this->DrawShapeCore(dc, x, y, shape, drawVertices, clipWidth, clipHeight, backColor);
	return ;
}

// *****************************************************************
//	   DrawShapeVB()
// *****************************************************************
void  CShapeDrawingOptions::DrawShapeVB(int hdc, float x, float y, CShape* shape, VARIANT_BOOL drawVertices, 
											   int clipWidth, int clipHeight, OLE_COLOR backColor, 
											   VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CDC* dc = CDC::FromHandle((HDC)hdc);

	*retVal = this->DrawShapeCore(dc, x, y, shape, drawVertices, clipWidth, clipHeight, backColor);
	return ;
}

// *****************************************************************
//	   DrawShapeCore()
// *****************************************************************
VARIANT_BOOL CShapeDrawingOptions::DrawShapeCore(CDC* dc, float x, float y, CShape* shape, VARIANT_BOOL drawVertices,
												 int clipWidth, int clipHeight, OLE_COLOR backColor)
{
	if (!dc)
	{
		_lastErrorCode = tkFAILED_TO_OBTAIN_DC;
		return VARIANT_FALSE;
	}
	if (!shape)
	{
		_lastErrorCode = tkUNEXPECTED_NULL_PARAMETER;
		return VARIANT_FALSE;
	}
	
	long numPoints;
	shape->get_NumPoints(&numPoints);
	if (numPoints == 0)	return VARIANT_FALSE;

	Gdiplus::Bitmap bmp(clipWidth, clipHeight, PixelFormat32bppARGB);
	Gdiplus::Graphics g(&bmp);

	// reading the points
	Gdiplus::Point* points = new Gdiplus::Point[numPoints];
	for (long i = 0; i < numPoints; i++)
	{
		double xVal, yVal;
		VARIANT_BOOL vbretval;
		shape->get_XY(i, &xVal, &yVal, &vbretval);
		points[i].X = (int)xVal;
		points[i].Y = (int)yVal;
	}
	
	Gdiplus::GraphicsPath path;
	path.AddLines(points, numPoints);

	// getting shape type
	ShpfileType shpType;
	shape->get_ShapeType(&shpType);
	ShpfileType simpleType =Utility::ShapeTypeConvert2D(shpType);
	
	// do we have line pattern here?
	bool usePattern = false;
	if (simpleType == SHP_POLYLINE)
	{
		usePattern = _options.useLinePattern && _options.CanUseLinePattern();
	}

	if (usePattern)
	{
		VARIANT_BOOL vbretval;
		HDC hdcTemp = g.GetHDC();
		_options.linePattern->Draw((int**)hdcTemp, 0.0f, 0.0f, clipWidth, clipHeight, backColor, &vbretval);
		g.ReleaseHDC(hdcTemp);
	}
	else
	{
		Gdiplus::Color clr(255 << 24 | BGR_TO_RGB(backColor));
		Gdiplus::SolidBrush brushBackground(clr);
		
		g.Clear(clr);
		
		// calculating bounding rectangle (for gradient)
		double xMin, xMax, yMin, yMax;
		((CShape*)shape)->get_ExtentsXY(xMin, yMin, xMax, yMax);

		Gdiplus::RectF rect((Gdiplus::REAL)(xMin), (Gdiplus::REAL)(yMin), (Gdiplus::REAL)(xMax - xMin), (Gdiplus::REAL)(yMax - yMin));
		
		// initializing
		if (simpleType != SHP_POINT)
		{
			_options.InitGdiPlusBrushAndPen(&rect);
			if (_options.penPlus)
				_options.penPlus->SetAlignment(Gdiplus::PenAlignmentInset);
		}
		
		// drawing
		Gdiplus::SmoothingMode mode;
		mode = g.GetSmoothingMode();

		if (simpleType == SHP_POLYGON)
		{
			g.SetSmoothingMode(Gdiplus::SmoothingModeDefault);

			if (numPoints == 2 )
			{
				Gdiplus::Rect r(points[0].X, points[0].Y, points[1].X, points[1].Y);
				
				if (_options.fillVisible)
				{
					g.FillRectangle(&brushBackground, r);
					g.FillRectangle(_options.brushPlus, r);
				}
				
				if (_options.penPlus)
					g.DrawRectangle(_options.penPlus, r);
			}
			else
			{
				if (_options.fillVisible)
				{
					g.FillPolygon(&brushBackground, points, numPoints);
					g.FillPolygon(_options.brushPlus, points, numPoints);
				}

				if (_options.penPlus)
					g.DrawPolygon(_options.penPlus, points, numPoints);
			}
		}
		else if (simpleType == SHP_POLYLINE)
		{
			g.SetSmoothingMode(Gdiplus::SmoothingModeDefault);

			if (_options.penPlus)
				g.DrawLines(_options.penPlus, points, numPoints);
		}
		else if (simpleType == SHP_POINT)
		{
			g.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
			
			_options.DrawPointSymbol(g, dc, points, NULL, 1);
		}

		// clearing
		g.SetSmoothingMode(mode);
		_options.ReleaseGdiPlusBrush();
		_options.ReleaseGdiPlusPen();
	}

	// -----------------------------------------
	// drawing of vertices
	// -----------------------------------------
	if ( _options.verticesVisible && drawVertices)
	{
		dc->EndPath();
		_options.InitGdiVerticesPen(dc);
		if (simpleType == SHP_POLYLINE || simpleType == SHP_POLYGON )
		{
			CPoint* square = _options.GetVertex();
			int size = _options.verticesSize/2;
			
			Gdiplus::SolidBrush brush(255<<24 | BGR_TO_RGB(_options.verticesColor));
			Gdiplus::Pen pen(255<<24 | BGR_TO_RGB(_options.verticesColor));

			for (int i = 0; i < numPoints; i++)
			{
				Gdiplus::Matrix mtx;
				mtx.Translate((Gdiplus::REAL)points[i].X, (Gdiplus::REAL)points[i].Y);
				g.SetTransform(&mtx);
			
				if ( _options.verticesType == vtSquare )
				{
					if (_options.verticesFillVisible )
					{
						g.FillPolygon(&brush, reinterpret_cast<Gdiplus::Point*>( square), 4);
					}
					else
					{
						g.DrawPolygon(&pen, reinterpret_cast<Gdiplus::Point*> (square), 4);
					}

				}
				else if ( _options.verticesType == vtCircle )
				{
					if (_options.verticesFillVisible)
					{
						g.FillEllipse(&brush, -size, -size, _options.verticesSize, _options.verticesSize);
					}
					else
					{
						g.DrawEllipse(&pen, -size, -size, _options.verticesSize, _options.verticesSize);
					}
				}

				mtx.Reset();
				g.SetTransform(&mtx);
			}
			delete[] square;
		}
		_options.ReleaseGdiBrushAndPen(dc);
	}
	
	Gdiplus::Graphics gResult(dc->GetSafeHdc());
	gResult.DrawImage(&bmp, x, y);
	
	delete[] points;

	return VARIANT_TRUE;
}
#pragma endregion

// *********************************************************
//		get_PointCharacter
// *********************************************************
void  CShapeDrawingOptions::get_PointCharacter (short* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = (short)_options.pointCharcter;
	return ;
}
void  CShapeDrawingOptions::put_PointCharacter (short newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_options.pointCharcter = (unsigned char)newVal;
	return ;
}

// *****************************************************************
//		FontName()
// *****************************************************************
void  CShapeDrawingOptions::get_FontName(BSTR* retval)					
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	*retval = A2BSTR(_options.fontName);
	return ;
};	
void  CShapeDrawingOptions::put_FontName(BSTR newVal)					
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	_options.fontName = OLE2CA(newVal);
	return ;
};	

// *****************************************************************
//		FontName()
// *****************************************************************
void  CShapeDrawingOptions::get_Tag(BSTR* retVal)					
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	*retVal = A2BSTR(_options.tag);
	return ;
};	
void  CShapeDrawingOptions::put_Tag(BSTR newVal)					
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	_options.tag = OLE2CA(newVal);
	return ;
};	

// *****************************************************************
//		Clone()
// *****************************************************************
void  CShapeDrawingOptions::Clone(CShapeDrawingOptions** retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CShapeDrawingOptions* sdo = new CShapeDrawingOptions();
	if (sdo)
	{
		((CShapeDrawingOptions*)sdo)->put_underlyingOptions(&_options);
	}
	*retval = sdo;
	return ;
}

// *****************************************************************
//		VerticesVisible()
// *****************************************************************
void  CShapeDrawingOptions::get_VerticesVisible(VARIANT_BOOL* retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retval = _options.verticesVisible;
	return ;
}
void  CShapeDrawingOptions::put_VerticesVisible(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	_options.verticesVisible = newVal;
	return ;
}

// *****************************************************************
//		VerticesSize()
// *****************************************************************
void  CShapeDrawingOptions::get_VerticesSize(LONG* retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retval = _options.verticesSize;
	return ;
}
void  CShapeDrawingOptions::put_VerticesSize(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	_options.verticesSize = newVal;
	return ;
}

// *****************************************************************
//		VerticesColor()
// *****************************************************************
void  CShapeDrawingOptions::get_VerticesColor(OLE_COLOR* retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retval = _options.verticesColor;
	return ;
}
void  CShapeDrawingOptions::put_VerticesColor(OLE_COLOR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	_options.verticesColor = newVal;
	return ;
}

// *****************************************************************
//		VerticesFillVisible()
// *****************************************************************
void  CShapeDrawingOptions::get_VerticesFillVisible(VARIANT_BOOL* retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retval = _options.verticesFillVisible;
	return ;
}
void  CShapeDrawingOptions::put_VerticesFillVisible(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	_options.verticesFillVisible = newVal;
	return ;
}

// *****************************************************************
//		VerticesType()
// *****************************************************************
void  CShapeDrawingOptions::get_VerticesType(tkVertexType* retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retval = _options.verticesType;
	return ;
}
void  CShapeDrawingOptions::put_VerticesType(tkVertexType newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	_options.verticesType = newVal;
	return ;
}

// *****************************************************************
//		get_FrameVisible()
// *****************************************************************
void  CShapeDrawingOptions::get_FrameVisible(VARIANT_BOOL* retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retval = _options.drawFrame;
	return ;
}
void  CShapeDrawingOptions::put_FrameVisible(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
		_options.drawFrame = newVal ? true: false;
	return ;
}

// *****************************************************************
//		get_FrameType()
// *****************************************************************
void  CShapeDrawingOptions::get_FrameType(tkLabelFrameType* retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retval = _options.frameType;
	return ;
}
void  CShapeDrawingOptions::put_FrameType(tkLabelFrameType newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	_options.frameType = newVal;
	return ;
}

// *****************************************************************
//		LinePattern ()
// *****************************************************************
void  CShapeDrawingOptions::get_LinePattern(CLinePattern** retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = _options.linePattern;
	if (_options.linePattern)
	{
		;///_options.linePattern->AddRef();
	}

	return ;
}
void  CShapeDrawingOptions::put_LinePattern(CLinePattern* newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
		_options.linePattern = newVal;
	return ;
}

// ****************************************************************
//		SetGradientFill
// ****************************************************************
void  CShapeDrawingOptions::SetGradientFill(OLE_COLOR color, short range)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if (range < 0)
	{
		ErrorMessage(tkINVALID_PARAMETER_VALUE);
	}
	unsigned char r = GetRValue(color);
	unsigned char g = GetGValue(color);
	unsigned char b = GetBValue(color);

	_options.fillColor = RGB(MAX(r - range/2, 0) , MAX(g - range/2, 0), MAX(b- range/2,0));
	_options.fillColor2 = RGB(MIN(r + range/2, 255) , MIN(g + range/2,255), MIN(b+ range/2,255));
	_options.fillType = ftGradient;
	return ;
}

// ****************************************************************
//		SetDefaultPointSymbol
// ****************************************************************
// Gives fast access to the most common shapes
void  CShapeDrawingOptions::SetDefaultPointSymbol (tkDefaultPointSymbol symbol)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	_options.pointSymbolType = ptSymbolStandard;
	_options.rotation = 0.0;

	tkPointShapeType type;
	double rotation;
	float sidesRatio;
	int numSides; 

	get_DefaultSymbolOptions(symbol, type, rotation, numSides, sidesRatio);
	
	_options.pointShapeType = type;
	_options.rotation = rotation;
	_options.pointNumSides = numSides;
	_options.pointShapeRatio = sidesRatio;

	return ;
}

#pragma region Properties

// ****************************************************************
//		get_Visible
// ****************************************************************
void  CShapeDrawingOptions::get_Visible(VARIANT_BOOL *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	*pVal = (VARIANT_BOOL)_options.visible; 	
	return ;
}
void  CShapeDrawingOptions::put_Visible(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	_options.visible = newVal?true:false;	
	return ;
}

// ****************************************************************
//		get_FillVisible
// ****************************************************************
void  CShapeDrawingOptions::get_FillVisible (VARIANT_BOOL *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _options.fillVisible;
	return ;
}
void  CShapeDrawingOptions::put_FillVisible (VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	_options.fillVisible = newVal?true:false;	
	return ;
}

// ****************************************************************
//		get_LineVisible
// ****************************************************************
void  CShapeDrawingOptions::get_LineVisible (VARIANT_BOOL *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	*pVal = _options.linesVisible; 				
	return ;
}
void  CShapeDrawingOptions::put_LineVisible (VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_options.linesVisible = newVal?true:false;	
	return ;
}

// ****************************************************************
//		get_FillColor
// ****************************************************************
void  CShapeDrawingOptions::get_FillColor (OLE_COLOR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	*pVal = _options.fillColor; 		
	return ;
}
void  CShapeDrawingOptions::put_FillColor (OLE_COLOR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_options.fillColor = newVal;
	return ;
}

// ****************************************************************
//		get_LineColor
// ****************************************************************
void  CShapeDrawingOptions::get_LineColor (OLE_COLOR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _options.lineColor;
	return ;
}
void  CShapeDrawingOptions::put_LineColor (OLE_COLOR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	_options.lineColor = newVal;		
	return ;
}

// ****************************************************************
//		get_DrawingMode
// ****************************************************************
void  CShapeDrawingOptions::get_DrawingMode (tkVectorDrawingMode *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _options.drawingMode;
	return ;
}
void  CShapeDrawingOptions::put_DrawingMode (tkVectorDrawingMode newVal)	
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	// It's no longer allowed to change drawing mode
	//m_options.drawingMode = newVal;
	return ;
}

// ****************************************************************
//		get_FillHatchStyle
// ****************************************************************
void  CShapeDrawingOptions::get_FillHatchStyle (tkGDIPlusHatchStyle *pVal)	
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	*pVal = _options.fillHatchStyle; 	
	return ;
}
void  CShapeDrawingOptions::put_FillHatchStyle (tkGDIPlusHatchStyle newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	if (newVal >= -1 && newVal <= 52)
	{
		_options.fillHatchStyle = newVal;	
	}
	else
	{
		ErrorMessage(tkINVALID_PARAMETER_VALUE);
	}
	return ;
}

// ****************************************************************
//		get_LineStipple
// ****************************************************************
void  CShapeDrawingOptions::get_LineStipple (tkDashStyle *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	*pVal = _options.lineStipple; 	
	return ;
}
void  CShapeDrawingOptions::put_LineStipple (tkDashStyle newVal)			
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	if (newVal >= 0 && newVal <= 5)
	{
		_options.lineStipple = newVal;	
	}
	else
	{
		ErrorMessage(tkINVALID_PARAMETER_VALUE);
	}
	return ;
}

// ****************************************************************
//		get_PointShape
// ****************************************************************
void  CShapeDrawingOptions::get_PointShape (tkPointShapeType *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _options.pointShapeType;
	return ;
}
void  CShapeDrawingOptions::put_PointShape (tkPointShapeType newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (newVal >= 0 && newVal <= 5)
	{
		_options.pointShapeType = newVal;	
	}
	else
	{
		ErrorMessage(tkINVALID_PARAMETER_VALUE);
	}
	return ;
}

// ****************************************************************
//		get_FillTransparency
// ****************************************************************
void  CShapeDrawingOptions::get_FillTransparency (float *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _options.fillTransparency;
	return ;
}
void  CShapeDrawingOptions::put_FillTransparency (float newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	if (newVal < 0) newVal = 0;
	if (newVal > 255) newVal = 255;
	_options.fillTransparency = newVal;	
	return ;
}

// ****************************************************************
//		get_LineWidth
// ****************************************************************
void  CShapeDrawingOptions::get_LineWidth (float *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	*pVal = _options.lineWidth; 			
	return ;
}
void  CShapeDrawingOptions::put_LineWidth (float newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	if (newVal < 1) newVal = 1;
	if (newVal > 20) newVal = 20;
	_options.lineWidth = newVal;		
	return ;
}

// ****************************************************************
//		get_PointSize
// ****************************************************************
void  CShapeDrawingOptions::get_PointSize (float *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	*pVal = _options.pointSize; 							
	return ;
}
void  CShapeDrawingOptions::put_PointSize (float newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	if (newVal < 1) newVal = 1;
	if (newVal > 100) newVal = 100;
	_options.pointSize = newVal;							
	return ;
}


// ****************************************************************
//		get_FillBgTransparent
// ****************************************************************
void  CShapeDrawingOptions::get_FillBgTransparent (VARIANT_BOOL* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	*pVal = _options.fillBgTransparent; 				
	return ;
}
void  CShapeDrawingOptions::put_FillBgTransparent (VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	_options.fillBgTransparent = newVal?true:false;	
	return ;
}

// ****************************************************************
//		get_FillBgColor
// ****************************************************************
void  CShapeDrawingOptions::get_FillBgColor (OLE_COLOR* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	*pVal = _options.fillBgColor; 					
	return ;
}
void  CShapeDrawingOptions::put_FillBgColor (OLE_COLOR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	_options.fillBgColor = newVal;					
	return ;
}

// ****************************************************************
//		get_FillType
// ****************************************************************
void  CShapeDrawingOptions::get_FillType (tkFillType* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	*pVal = _options.fillType; 	
	return ;
}
void  CShapeDrawingOptions::put_FillType (tkFillType newVal)					
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	if (newVal >= 0 && newVal <= 3)
	{
		_options.fillType = newVal;		
	}
	else
	{
		ErrorMessage(tkINVALID_PARAMETER_VALUE);
	}
	return ;
}

// ****************************************************************
//		get_FillGradientType
// ****************************************************************
void  CShapeDrawingOptions::get_FillGradientType (tkGradientType* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	*pVal = _options.fillGradientType; 	
	return ;
}
void  CShapeDrawingOptions::put_FillGradientType (tkGradientType newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	if (newVal >= 0 && newVal <= 2)
	{
		_options.fillGradientType = newVal;	
	}
	else
	{
		ErrorMessage(tkINVALID_PARAMETER_VALUE);
	}
	return ;
}

// ****************************************************************
//		get_PointType
// ****************************************************************
void  CShapeDrawingOptions::get_PointType(tkPointSymbolType* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _options.pointSymbolType;
	return ;
}
void  CShapeDrawingOptions::put_PointType (tkPointSymbolType newVal)			
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	if (newVal >= 0 && newVal <= 2)
	{
		_options.pointSymbolType = newVal;
	}
	else
	{
		ErrorMessage(tkINVALID_PARAMETER_VALUE);
	}
	return ;
}

// ****************************************************************
//		get_FillColor2
// ****************************************************************
void  CShapeDrawingOptions::get_FillColor2 (OLE_COLOR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	*pVal = _options.fillColor2; 		
	return ;
}
void  CShapeDrawingOptions::put_FillColor2 (OLE_COLOR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	_options.fillColor2 = newVal;		
	return ;
}

// ****************************************************************
//		get_PointRotation
// ****************************************************************
void  CShapeDrawingOptions::get_PointRotation (double *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	*pVal = _options.rotation; 		
	return ;
}
void  CShapeDrawingOptions::put_PointRotation (double newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	if (newVal > 360.0 || newVal < -360.0)
	{
		ErrorMessage(tkINVALID_PARAMETER_VALUE);
	}
	else
	{
		_options.rotation = newVal;		
	}
	return ;
}

// ****************************************************************
//		get_PointSidesCount
// ****************************************************************
void  CShapeDrawingOptions::get_PointSidesCount (long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	*pVal = _options.pointNumSides;	
	return ;
}
void  CShapeDrawingOptions::put_PointSidesCount (long newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	if (newVal > 20) newVal = 20;
	if (newVal < 2) newVal = 2;
	_options.pointNumSides = newVal;	
	return ;
}

// ****************************************************************
//		get_PointSidesRatio
// ****************************************************************
void  CShapeDrawingOptions::get_PointSidesRatio (float *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _options.pointShapeRatio;
	return ;
}
void  CShapeDrawingOptions::put_PointSidesRatio (float newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	if (newVal < 0.1f) newVal = 0.1f;
	if (newVal > 1.0f) newVal = 1.0f;
	_options.pointShapeRatio = newVal;
	return ;
}

// ****************************************************************
//		get_PointSidesRatio
// ****************************************************************
void  CShapeDrawingOptions::get_FillRotation(double *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	*pVal = _options.fillGradientRotation;	
	return ;
}
void  CShapeDrawingOptions::put_FillRotation (double newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	if (newVal > 360.0 || newVal < -360.0)
	{
		ErrorMessage(tkINVALID_PARAMETER_VALUE);
	}
	else
	{
		_options.fillGradientRotation = newVal;
	}
	return ;
}

// ****************************************************************
//		get_FillGradientBounds
// ****************************************************************
void  CShapeDrawingOptions::get_FillGradientBounds (tkGradientBounds *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	*pVal = _options.fillGradientBounds;	
	return ;
}
void  CShapeDrawingOptions::put_FillGradientBounds (tkGradientBounds newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	if (newVal >= 0 && newVal <= 1)
	{
		_options.fillGradientBounds = newVal;	
	}
	else
	{
		ErrorMessage(tkINVALID_PARAMETER_VALUE);
	}
	return ;
}

// ****************************************************************
//		get_LineTransparency
// ****************************************************************
void  CShapeDrawingOptions::get_LineTransparency(float *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _options.lineTransparency;
	return ;
}
void  CShapeDrawingOptions::put_LineTransparency(float newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	if (newVal < 0) newVal = 0;
	if (newVal > 255) newVal = 255;
	_options.lineTransparency = newVal;	
	return ;
}

// ****************************************************************
//		get_PictureScaleX
// ****************************************************************
void  CShapeDrawingOptions::get_PictureScaleX(double *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	*pVal = _options.scaleX; 	
	return ;
}
void  CShapeDrawingOptions::put_PictureScaleX(double newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	if (newVal < 0.1) newVal = 0.1;
	if (newVal > 5.0) newVal = 5.0;
	_options.scaleX = newVal;	
	return ;
}

// ****************************************************************
//		get_PictureScaleY
// ****************************************************************
void  CShapeDrawingOptions::get_PictureScaleY(double *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	*pVal = _options.scaleY; 	
	return ;
}
void  CShapeDrawingOptions::put_PictureScaleY(double newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	if (newVal < 0.1) newVal = 0.1;
	if (newVal > 5.0) newVal = 5.0;
	_options.scaleY = newVal;	
	return ;
}
#pragma endregion

// ****************************************************************
//		get_AlignPictureBottom
// ****************************************************************
void  CShapeDrawingOptions::get_AlignPictureByBottom(VARIANT_BOOL *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	*pVal = _options.alignIconByBottom; 	
	return ;
}
void  CShapeDrawingOptions::put_AlignPictureByBottom(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	_options.alignIconByBottom = newVal ? true: false;	
	return ;
}

// ********************************************************
//     Serialize()
// ********************************************************
void  CShapeDrawingOptions::Serialize(BSTR* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	CPLXMLNode* node = SerializeCore("ShapeDrawingOptionsClass");
	Utility::SerializeAndDestroyXmlTree(node, retVal);
	return ;
}

// ********************************************************
//     SerializeCore()
// ********************************************************
CPLXMLNode* CShapeDrawingOptions::SerializeCore(CString ElementName)
{
	USES_CONVERSION;
	
	CPLXMLNode* psTree = CPLCreateXMLNode( NULL, CXT_Element, ElementName);
	CString str;
	
	CDrawingOptionsEx* opt = new CDrawingOptionsEx();

	if (opt->fillBgColor != _options.fillBgColor)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "FillBgColor", CPLString().Printf("%d", _options.fillBgColor));

	if (opt->fillBgTransparent != _options.fillBgTransparent)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "FillBgTransparent", CPLString().Printf("%d", (int)_options.fillBgTransparent));

	if (opt->fillColor != _options.fillColor)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "FillColor", CPLString().Printf("%d", _options.fillColor));

	if (opt->fillColor2 != _options.fillColor2)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "FillColor2", CPLString().Printf("%d", _options.fillColor2));
	
	if (opt->fillGradientBounds != _options.fillGradientBounds)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "FillGradientBounds", CPLString().Printf("%d", (int)_options.fillGradientBounds));
	
	if (opt->fillGradientRotation != _options.fillGradientRotation)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "FillGradientRotation", CPLString().Printf("%f", _options.fillGradientRotation));
	
	if (opt->fillGradientType != _options.fillGradientType)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "FillGradientType", CPLString().Printf("%d", (int)_options.fillGradientType));
	
	if (opt->fillHatchStyle != _options.fillHatchStyle)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "FillHatchStyle", CPLString().Printf("%d", (int)_options.fillHatchStyle));
	
	if (opt->fillTransparency != _options.fillTransparency)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "FillTransparency", CPLString().Printf("%f", _options.fillTransparency));
	
	if (opt->fillType != _options.fillType)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "FillType", CPLString().Printf("%d", (int)_options.fillType));
	
	if (opt->fillVisible != _options.fillVisible)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "FillVisible", CPLString().Printf("%d", (int)_options.fillVisible));
	
	if (opt->fontName != _options.fontName)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "FontName", _options.fontName);
	
	if (opt->lineColor != _options.lineColor)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "LineColor", CPLString().Printf("%d", _options.lineColor));

	if (opt->lineTransparency != _options.lineTransparency)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "LineTransparency", CPLString().Printf("%f", _options.lineTransparency));
	
	if (opt->lineStipple != _options.lineStipple)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "LineStipple", CPLString().Printf("%d", (int)_options.lineStipple));
	
	if (opt->linesVisible != _options.linesVisible)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "LinesVisible", CPLString().Printf("%d", (int)_options.linesVisible));
	
	if (opt->lineWidth != _options.lineWidth)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "LineWidth", CPLString().Printf("%f", _options.lineWidth));
	
	if (opt->pointCharcter != _options.pointCharcter)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "PointCharcter", CPLString().Printf("%d", (int)_options.pointCharcter));
	
	if (opt->pointColor != _options.pointColor)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "PointColor", CPLString().Printf("%d", _options.pointColor));
	
	if (opt->pointNumSides != _options.pointNumSides)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "PointNumSides", CPLString().Printf("%d", _options.pointNumSides));
	
	if (opt->pointShapeRatio != _options.pointShapeRatio)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "PointShapeRatio", CPLString().Printf("%f", _options.pointShapeRatio));
	
	if (opt->pointShapeType != _options.pointShapeType)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "PointShapeType", CPLString().Printf("%d", (int)_options.pointShapeType));
	
	if (opt->pointSize != _options.pointSize)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "PointSize", CPLString().Printf("%f", _options.pointSize));
	
	if (opt->pointSymbolType != _options.pointSymbolType)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "PointSymbolType", CPLString().Printf("%d", (int)_options.pointSymbolType));
	
	if (opt->rotation != _options.rotation)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "Rotation", CPLString().Printf("%f", _options.rotation));
	
	if (opt->scaleX != _options.scaleX)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "ScaleX", CPLString().Printf("%f", _options.scaleX));
	
	if (opt->scaleY != _options.scaleY)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "ScaleY", CPLString().Printf("%f", _options.scaleY));
	
	if (opt->verticesColor != _options.verticesColor)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "VerticesColor", CPLString().Printf("%d", _options.verticesColor));
	
	if (opt->verticesFillVisible != _options.verticesFillVisible)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "VerticesFillVisible", CPLString().Printf("%d", (int)_options.verticesFillVisible));
	
	if (opt->verticesSize != _options.verticesSize)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "VerticesSize", CPLString().Printf("%d", _options.verticesSize));
	
	if (opt->verticesType != _options.verticesType)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "VerticesType", CPLString().Printf("%d", (int)_options.verticesType));
	
	if (opt->verticesVisible != _options.verticesVisible)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "VerticesVisible", CPLString().Printf("%d", (int)_options.verticesVisible));
		
	if (opt->visible != _options.visible)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "Visible", CPLString().Printf("%d", (int)_options.visible));

	if (opt->useLinePattern != _options.useLinePattern)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "UseLinePattern", CPLString().Printf("%d", (int)_options.useLinePattern));

	if (opt->alignIconByBottom != _options.alignIconByBottom)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "AlignPictureByBottom", CPLString().Printf("%d", (int)_options.alignIconByBottom));

	if (opt->drawFrame != _options.drawFrame)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "FrameVisible", CPLString().Printf("%d", (int)_options.drawFrame));

	if (opt->frameType != _options.frameType)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "FrameType", CPLString().Printf("%d", (int)_options.frameType));

	if (opt->dynamicVisibility != _options.dynamicVisibility)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "DynamicVisibility", CPLString().Printf("%d", (int)_options.dynamicVisibility));
	
	if (opt->minVisibleScale != _options.minVisibleScale)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "MinVisibleScale", CPLString().Printf("%f", _options.minVisibleScale));
	
	if (opt->maxVisibleScale != _options.maxVisibleScale)
		Utility::CPLCreateXMLAttributeAndValue(psTree, "MaxVisibleScale", CPLString().Printf("%f", _options.maxVisibleScale));
	
	delete opt;

	if (_options.linePattern)
	{
		CPLXMLNode* psNode = ((CLinePattern*)_options.linePattern)->SerializeCore("LinePatternClass");
		if (psNode)
		{
			CPLAddXMLChild(psTree, psNode);
		}
	}
	
	if (_options.picture)
	{
		CPLXMLNode* psNode = ((CImageClass*)_options.picture)->SerializeCore(VARIANT_TRUE, "Picture");
		if (psNode)
		{
			CPLAddXMLChild(psTree, psNode);
		}
	}
	
	return psTree;
}

// ********************************************************
//     DeserializeCore()
// ********************************************************
bool CShapeDrawingOptions::DeserializeCore(CPLXMLNode* node)
{
	if (!node)
		return false;
	
	// to set default options to the values that were not serialized
	CDrawingOptionsEx* opt = new CDrawingOptionsEx();

	CString s;
	
	s = CPLGetXMLValue( node, "FillBgColor", NULL );
	_options.fillBgColor = (s == "") ? opt->fillBgColor : (OLE_COLOR)atoi(s.GetString());

	s = CPLGetXMLValue( node, "FillBgTransparent", NULL );
	_options.fillBgTransparent = (s == "") ? opt->fillBgTransparent : atoi(s.GetString()) == 0 ? false : true;

	s = CPLGetXMLValue( node, "FillColor", NULL );
	_options.fillColor = (s == "") ? opt->fillColor : (OLE_COLOR)atoi(s.GetString());

	s = CPLGetXMLValue( node, "FillColor2", NULL );
	_options.fillColor2 = (s == "") ? opt->fillColor2 : (OLE_COLOR)atoi(s.GetString());

	s = CPLGetXMLValue( node, "FillGradientBounds", NULL );
	_options.fillGradientBounds = (s == "") ? opt->fillGradientBounds : (tkGradientBounds)atoi(s.GetString());

	s = CPLGetXMLValue( node, "FillGradientRotation", NULL );
	_options.fillGradientRotation = (s == "") ? opt->fillGradientRotation : Utility::atof_custom(s);

	s = CPLGetXMLValue( node, "FillGradientType", NULL );
	_options.fillGradientType = (s == "") ? opt->fillGradientType : (tkGradientType)atoi(s.GetString());
	
	s = CPLGetXMLValue( node, "FillHatchStyle", NULL );
	_options.fillHatchStyle = (s == "") ? opt->fillHatchStyle : (tkGDIPlusHatchStyle)atoi(s.GetString());

	s = CPLGetXMLValue( node, "FillTransparency", NULL );
	_options.fillTransparency = (s == "") ? opt->fillTransparency : (float)Utility::atof_custom(s);

	s = CPLGetXMLValue( node, "FillType", NULL );
	_options.fillType = (s == "") ? opt->fillType : (tkFillType)atoi(s.GetString());
	
	s = CPLGetXMLValue( node, "FillVisible", NULL );
	_options.fillVisible = (s == "") ? opt->fillVisible : atoi(s.GetString()) == 0 ? false : true;

	s = CPLGetXMLValue( node, "FontName", NULL );
	_options.fontName = (s == "") ? opt->fontName : s.GetString();

	s = CPLGetXMLValue( node, "LineColor", NULL );
	_options.lineColor = (s == "") ? opt->lineColor : (OLE_COLOR)atoi(s.GetString());
	
	s = CPLGetXMLValue( node, "LineStipple", NULL );
	_options.lineStipple = (s == "") ? opt->lineStipple : (tkDashStyle)atoi(s.GetString());

	s = CPLGetXMLValue( node, "LineTransparency", NULL );
	_options.lineTransparency = (s == "") ? opt->lineTransparency : (float)Utility::atof_custom(s);

	s = CPLGetXMLValue( node, "LinesVisible", NULL );
	_options.linesVisible = (s == "") ? opt->linesVisible : atoi(s.GetString()) == 0 ? false : true;

	s = CPLGetXMLValue( node, "LineWidth", NULL );
	_options.lineWidth = (s == "") ? opt->lineWidth : (float)Utility::atof_custom(s);

	s = CPLGetXMLValue( node, "PointCharcter", NULL );
	_options.pointCharcter = (s == "") ? opt->pointCharcter : (unsigned char)atoi(s.GetString());

	s = CPLGetXMLValue( node, "PointColor", NULL );
	_options.pointColor = (s == "") ? opt->pointColor : (OLE_COLOR)atoi(s.GetString());
	
	s = CPLGetXMLValue( node, "PointNumSides", NULL );
	_options.pointNumSides = (s == "") ? opt->pointNumSides : atoi(s.GetString());

	s = CPLGetXMLValue( node, "PointShapeRatio", NULL );
	_options.pointShapeRatio = (s == "") ? opt->pointShapeRatio : (float)Utility::atof_custom(s);

	s = CPLGetXMLValue( node, "PointShapeType", NULL );
	_options.pointShapeType = (s == "") ? opt->pointShapeType : (tkPointShapeType)atoi(s.GetString());
	
	s = CPLGetXMLValue( node, "PointSize", NULL );
	_options.pointSize = (s == "") ? opt->pointSize : (float)Utility::atof_custom(s);

	s = CPLGetXMLValue( node, "PointSymbolType", NULL );
	_options.pointSymbolType = (s == "") ? opt->pointSymbolType : (tkPointSymbolType)atoi(s.GetString());
	
	s = CPLGetXMLValue( node, "Rotation", NULL );
	_options.rotation = (s == "") ? opt->rotation : Utility::atof_custom(s);
	
	s = CPLGetXMLValue( node, "ScaleX", NULL );
	_options.scaleX = (s == "") ? opt->scaleX : Utility::atof_custom(s);
	
	s = CPLGetXMLValue( node, "ScaleY", NULL );
	_options.scaleY = (s == "") ? opt->scaleY : Utility::atof_custom(s);
	
	s = CPLGetXMLValue( node, "VerticesColor", NULL );
	_options.verticesColor = (s == "") ? opt->verticesColor : atoi(s);

	s = CPLGetXMLValue( node, "VerticesFillVisible", NULL );
	_options.verticesFillVisible = (s == "") ? opt->verticesFillVisible : (VARIANT_BOOL)atoi(s.GetString());

	s = CPLGetXMLValue( node, "VerticesSize", NULL );
	_options.verticesSize = (s == "") ? opt->verticesSize : atoi(s.GetString());

	s = CPLGetXMLValue( node, "VerticesType", NULL );
	_options.verticesSize = (s == "") ? opt->verticesSize : (tkVertexType)atoi(s.GetString());

	s = CPLGetXMLValue( node, "VerticesVisible", NULL );
	_options.verticesVisible = (s == "") ? opt->verticesVisible : (VARIANT_BOOL)atoi(s.GetString());
	
	s = CPLGetXMLValue( node, "Visible", NULL );
	_options.visible = (s == "") ? opt->visible : atoi(s.GetString()) == 0 ? false : true;
	
	s = CPLGetXMLValue( node, "AlignPictureByBottom", NULL );
	_options.alignIconByBottom = (s == "") ? opt->alignIconByBottom : atoi(s.GetString()) == 0 ? false : true;

	s = CPLGetXMLValue( node, "FrameVisible", NULL );
	_options.drawFrame = (s == "") ? opt->drawFrame : atoi(s.GetString()) == 0 ? false : true;

	s = CPLGetXMLValue( node, "FrameType", NULL );
	_options.frameType = (s == "") ? opt->frameType : (tkLabelFrameType)atoi(s.GetString());

	s = CPLGetXMLValue(node, "DynamicVisibility", NULL);
	_options.dynamicVisibility = (s == "") ? opt->dynamicVisibility : atoi(s.GetString()) == 0 ? false : true;
	
	s = CPLGetXMLValue(node, "MinVisibleScale", NULL);
	_options.minVisibleScale = (s == "") ? opt->minVisibleScale : Utility::atof_custom(s);
	
	s = CPLGetXMLValue(node, "MaxVisibleScale", NULL);
	_options.maxVisibleScale = (s == "") ? opt->maxVisibleScale : Utility::atof_custom(s);

	// restoring picture
	CPLXMLNode* psChild = CPLGetXMLNode(node, "Picture");
	if (psChild)
	{
		CImageClass * img = NULL;
		img = new CImageClass();//CoCreateInstance(CLSID_Image,NULL,CLSCTX_INPROC_SERVER,IID_IImage,(void**)&img);
		if (img)
		{
			((CImageClass*)img)->DeserializeCore(psChild);
			this->put_Picture(img);
		}
	}

	// restoring line pattern
	psChild = CPLGetXMLNode(node, "LinePatternClass");
	if (psChild)
	{
		CLinePattern* pattern = NULL;
		pattern = new CLinePattern();
		//CoCreateInstance(CLSID_LinePattern,NULL,CLSCTX_INPROC_SERVER,IID_ILinePattern,(void**)&pattern);
		if (pattern)
		{
			((CLinePattern*)pattern)->DeserializeCore(psChild);
			this->put_LinePattern(pattern);
		}
	}
	else
	{
		if (_options.linePattern)
		{
			_options.linePattern->Clear();
			delete _options.linePattern;
			_options.linePattern = NULL;
		}
	}

	s = CPLGetXMLValue( node, "UseLinePattern", NULL );
	_options.useLinePattern = (s == "") ? opt->useLinePattern : atoi(s.GetString()) == 0 ? false : true;

	delete opt;

	return true;
}

// ********************************************************
//     Deserialize()
// ********************************************************
void  CShapeDrawingOptions::Deserialize(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;

	CString s = OLE2CA(newVal);
	CPLXMLNode* node = CPLParseXMLString(s.GetString());
	if (node)
	{
		CPLXMLNode* nodeSdo = CPLGetXMLNode(node, "=ShapeDrawingOptionsClass");
		if (nodeSdo)
		{
			DeserializeCore(nodeSdo);
		}
		CPLDestroyXMLNode(node);
	}
	return ;
}

// ****************************************************************
//		get_MinScale
// ****************************************************************
void  CShapeDrawingOptions::get_MinScale (double *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	*pVal = _options.minScale; 			
	return ;
}
void  CShapeDrawingOptions::put_MinScale (double newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	_options.minScale = newVal;		
	return ;
}

// ****************************************************************
//		get_MaxScale
// ****************************************************************
void  CShapeDrawingOptions::get_MaxScale (double *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	*pVal = _options.maxScale; 			
	return ;
}
void  CShapeDrawingOptions::put_MaxScale (double newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	_options.maxScale = newVal;		
	return ;
}

// ****************************************************************
//		get_MinScale
// ****************************************************************
void  CShapeDrawingOptions::get_MinLineWidth (double *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	*pVal = _options.minLineWidth; 			
	return ;
}
void  CShapeDrawingOptions::put_MinLineWidth (double newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	_options.minLineWidth = newVal;		
	return ;
}

// ****************************************************************
//		get_MaxScale
// ****************************************************************
void  CShapeDrawingOptions::get_MaxLineWidth (double *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	*pVal = _options.maxLineWidth; 			
	return ;
}
void  CShapeDrawingOptions::put_MaxLineWidth (double newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	_options.maxLineWidth = newVal;		
	return ;
}

// ****************************************************************
//		get_DynamicVisibility
// ****************************************************************
void  CShapeDrawingOptions::get_DynamicVisibility(VARIANT_BOOL* pVal)
 {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _options.dynamicVisibility ? VARIANT_TRUE : VARIANT_FALSE;
	return ;
};
void  CShapeDrawingOptions::put_DynamicVisibility(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_options.dynamicVisibility = newVal == VARIANT_TRUE;
	return ;
};

// ****************************************************************
//		get_MinVisibleScale
// ****************************************************************
void  CShapeDrawingOptions::get_MinVisibleScale(DOUBLE* pVal)
 {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _options.minVisibleScale;
	return ;
};
void  CShapeDrawingOptions::put_MinVisibleScale(DOUBLE newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_options.minVisibleScale = newVal;
	return ;
};

// ****************************************************************
//		get_MaxVisibleScale
// ****************************************************************
void  CShapeDrawingOptions::get_MaxVisibleScale(DOUBLE* pVal)
 {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _options.maxVisibleScale;
	return ;
};
void  CShapeDrawingOptions::put_MaxVisibleScale(DOUBLE newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_options.maxVisibleScale = newVal;
	return ;
};

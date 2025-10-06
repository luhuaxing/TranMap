#include "stdafx.h"
#include "LineSegment.h"
#include "GdiPlus.h"
#include "DrawingOptions.h"
#include "ShapeDrawingOptions.h"
#include "macros.h"
#include "PointSymbols.h"

// *************************************************************
//		get_Color()
// *************************************************************
void  CLineSegment::get_Color(OLE_COLOR* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = _lineColor;
	return ;
}
void  CLineSegment::put_Color(OLE_COLOR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	_lineColor = newVal;
	return ;
}

// *************************************************************
//		get_MarkerOutlineColor()
// *************************************************************
void  CLineSegment::get_MarkerOutlineColor(OLE_COLOR* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = _markerOutlineColor;
	return ;
}
void  CLineSegment::put_MarkerOutlineColor(OLE_COLOR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	_markerOutlineColor = newVal;
	return ;
}

// *************************************************************
//		get_LineWidth()
// *************************************************************
void  CLineSegment::get_LineWidth(float* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = _lineWidth;
	return ;
}
void  CLineSegment::put_LineWidth(float newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	_lineWidth = newVal;
	return ;
}

// *************************************************************
//		get_LineStyle()
// *************************************************************
void  CLineSegment::get_LineStyle(tkDashStyle* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = _lineStyle;
	return ;
}
void  CLineSegment::put_LineStyle(tkDashStyle newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	_lineStyle = newVal;
	return ;
}

// *************************************************************
//		get_LineType()
// *************************************************************
void  CLineSegment::get_LineType(tkLineType* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = _lineType;
	return ;
}
void  CLineSegment::put_LineType(tkLineType newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	_lineType = newVal;
	return ;
}

// *************************************************************
//		get_Marker()
// *************************************************************
void  CLineSegment::get_Marker(tkDefaultPointSymbol* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = _marker;
	return ;
}
void  CLineSegment::put_Marker(tkDefaultPointSymbol newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	_marker = newVal;
	return ;
}

// *************************************************************
//		get_MarkerSize()
// *************************************************************
void  CLineSegment::get_MarkerSize(float* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = _markerSize;
	return ;
}
void  CLineSegment::put_MarkerSize(float newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	_markerSize = newVal;
	return ;
}

// *************************************************************
//		get_MarkerInterval()
// *************************************************************
void  CLineSegment::get_MarkerInterval(float* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = _markerInterval;
	return ;
}
void  CLineSegment::put_MarkerInterval(float newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	_markerInterval = newVal;
	return ;
}

// *************************************************************
//		get_MarkerOrientation()
// *************************************************************
void  CLineSegment::get_MarkerOrientation(tkLineLabelOrientation* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = _markerOrientation;
	return ;
}
void  CLineSegment::put_MarkerOrientation(tkLineLabelOrientation newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	_markerOrientation = newVal;
	return ;
}

// *************************************************************
//		get_MarkerFlipFirst()
// *************************************************************
void  CLineSegment::get_MarkerFlipFirst(VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = _markerFlipFirst;
	return ;
}
void  CLineSegment::put_MarkerFlipFirst(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	_markerFlipFirst = newVal;
	return ;
}
	
// *************************************************************
//		get_MarkerOffset()
// *************************************************************
void  CLineSegment::get_MarkerOffset(float* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = _markerOffset;
	return ;
}
void  CLineSegment::put_MarkerOffset(float newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	_markerOffset = newVal;
	return ;
}

// *************************************************************
//		Draw()
// *************************************************************
void  CLineSegment::Draw (int** hdc, float x, float y, int clipWidth, int clipHeight, OLE_COLOR backColor, VARIANT_BOOL* retVal)
{
	if (!hdc) {
		CallbackHelper::ErrorMsg("Invalid handle of device context.");
		return ;
	}

	CDC* dc = CDC::FromHandle((HDC)hdc);
	*retVal = this->DrawCore(dc, x, y, clipWidth, clipHeight, backColor);
	return ;
}

// *************************************************************
//		Draw()
// *************************************************************
void  CLineSegment::DrawVB (int hdc, float x, float y, int clipWidth, int clipHeight, OLE_COLOR backColor, VARIANT_BOOL* retVal)
{
	if (!hdc) {
		CallbackHelper::ErrorMsg("Invalid handle of device context.");
		return ;
	}

	CDC* dc = CDC::FromHandle((HDC)hdc);
	*retVal = this->DrawCore(dc, x, y, clipWidth, clipHeight, backColor);
	return ;
}

// *************************************************************
//		DrawCore()
// *************************************************************
VARIANT_BOOL CLineSegment::DrawCore(CDC* dc, float x, float y, int clipWidth, int clipHeight, OLE_COLOR backColor )
{
	if (!dc) {
		CallbackHelper::ErrorMsg("Invalid handle of device context.");
		return true ;
	}

	Gdiplus::Bitmap bmp(clipWidth, clipHeight, PixelFormat32bppARGB);
	Gdiplus::Graphics g(&bmp);
	g.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

	float xStart = 0.0f, yStart = 0.0f;

	Gdiplus::PointF points[2];
	points[0].X = 0.0f;
	points[0].Y = (float)clipHeight/2.0f;
	points[1].X = (float)clipWidth;
	points[1].Y = (float)clipHeight/2.0f;

	Gdiplus::GraphicsPath path;
	path.AddLines(&(points[0]), 2);

	Gdiplus::Color clr(255 << 24 | BGR_TO_RGB(backColor));
	Gdiplus::SolidBrush brushBackground(clr);
	g.Clear(clr);

	if(_lineWidth != 0) 
	{
		if (_lineType == lltSimple)
		{
			Gdiplus::Pen* pen = new Gdiplus::Pen(Gdiplus::Color(255 << 24 | BGR_TO_RGB(_lineColor)), _lineWidth);
			switch (_lineStyle)
			{
				case dsSolid:		pen->SetDashStyle(Gdiplus::DashStyleSolid);		break;
				case dsDash:		pen->SetDashStyle(Gdiplus::DashStyleDash);		break;
				case dsDot:			pen->SetDashStyle(Gdiplus::DashStyleDot);		break;
				case dsDashDotDot:	pen->SetDashStyle(Gdiplus::DashStyleDashDotDot);break;
				case dsDashDot:		pen->SetDashStyle(Gdiplus::DashStyleDashDot);	break;
				default:			pen->SetDashStyle(Gdiplus::DashStyleSolid);
			}
			
			g.DrawLines(pen, points, 2);
			delete pen;
		}
		else
		{
			int numPoints = 0;
			float* points = get_SimplePointShape(_marker, _markerSize, &numPoints);
			float offset = _markerOffset + x;

			if (numPoints > 0)
			{
				Gdiplus::SolidBrush* brush = new Gdiplus::SolidBrush(Gdiplus::Color(255 << 24 | BGR_TO_RGB(_lineColor)));
				Gdiplus::Pen* pen = new Gdiplus::Pen(Gdiplus::Color(255 << 24 | BGR_TO_RGB(_markerOutlineColor)));
				pen->SetAlignment(Gdiplus::PenAlignmentInset);

				g.TranslateTransform(offset, clipHeight/2.0f);
				while(offset < clipWidth)
				{
					g.FillPolygon(brush, (Gdiplus::PointF*)points, numPoints);
					g.DrawPolygon(pen, (Gdiplus::PointF*)points, numPoints);
					offset += _markerInterval;
					g.TranslateTransform(_markerInterval, 0.0f);
				}
				g.ResetTransform();
				delete[] points;
				delete brush;
				delete pen;
			}
		}
	}


	Gdiplus::Graphics gResult(dc->GetSafeHdc());
	gResult.DrawImage(&bmp, x, y);

	return  VARIANT_TRUE;
}

// ***********************************************************
//	   Draw()
// ***********************************************************
void  CLineSegment::Draw(Gdiplus::Graphics& g, BYTE transparency, int ImageWidth, int ImageHeight, int xOrigin, int yOrigin, VARIANT_BOOL* retVal)
{
	float xStart = 0.0f, yStart = 0.0f;

	Gdiplus::PointF points[2];
	points[0].X = 0.0f;// + xOrigin;
	points[0].Y = (float)ImageHeight/2.0f;// + yOrigin;
	points[1].X = (float)ImageWidth;// + xOrigin;
	points[1].Y = (float)ImageHeight/2.0f;// + yOrigin;

	Gdiplus::GraphicsPath path;
	path.AddLines(&(points[0]), 2);

	if(_lineWidth != 0) 
	{
		if (_lineType == lltSimple)
		{
			Gdiplus::Pen* pen = new Gdiplus::Pen(Gdiplus::Color(transparency << 24 | BGR_TO_RGB(_lineColor)), _lineWidth);
			switch (_lineStyle)
			{
				case dsSolid:		pen->SetDashStyle(Gdiplus::DashStyleSolid);		break;
				case dsDash:		pen->SetDashStyle(Gdiplus::DashStyleDash);		break;
				case dsDot:			pen->SetDashStyle(Gdiplus::DashStyleDot);		break;
				case dsDashDotDot:	pen->SetDashStyle(Gdiplus::DashStyleDashDotDot);break;
				case dsDashDot:		pen->SetDashStyle(Gdiplus::DashStyleDashDot);	break;
				default:			pen->SetDashStyle(Gdiplus::DashStyleSolid);
			}
			
			g.DrawLines(pen, points, 2);
			delete pen;
		}
		else
		{
			int numPoints = 0;
			float* points = get_SimplePointShape(_marker, _markerSize, &numPoints);
			float offset = _markerOffset;

			if (numPoints > 0)
			{
				Gdiplus::SolidBrush* brush = new Gdiplus::SolidBrush(Gdiplus::Color(transparency << 24 | BGR_TO_RGB(_lineColor)));
				Gdiplus::Pen* pen = new Gdiplus::Pen(Gdiplus::Color(transparency << 24 | BGR_TO_RGB(_markerOutlineColor)));
				pen->SetAlignment(Gdiplus::PenAlignmentInset);

				g.TranslateTransform(offset, ImageHeight/2.0f);
				while(offset < ImageWidth)
				{
					g.FillPolygon(brush, (Gdiplus::PointF*)points, numPoints);
					g.DrawPolygon(pen, (Gdiplus::PointF*)points, numPoints);
					offset += _markerInterval;
					g.TranslateTransform(_markerInterval, 0.0f);
				}
				g.ResetTransform();
				delete[] points;
				delete brush;
				delete pen;
			}
		}
	}
	*retVal = VARIANT_TRUE;
	return ;
}
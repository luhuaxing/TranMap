#include "stdafx.h"
#include "LinePattern.h"
#include "LineSegment.h"
#include "macros.h"
#include "PointSymbols.h"

#pragma region General
// *************************************************************
//		get_LastErrorCode
// *************************************************************
void  CLinePattern::get_LastErrorCode(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*pVal = _lastErrorCode;
	_lastErrorCode = tkNO_ERROR;
	return ;
}

// *************************************************************
//		get_ErrorMsg
// *************************************************************
void  CLinePattern::get_ErrorMsg(long ErrorCode, BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	*pVal = A2BSTR(ErrorMsg(ErrorCode));
	return ;
}

// *************************************************************
//		get/put_GlobalCallback
// *************************************************************
void  CLinePattern::get_GlobalCallback(ICallback **pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*pVal = _globalCallback;
	return ;
}
void  CLinePattern::put_GlobalCallback(ICallback *newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	_globalCallback = newVal;
	return ;
}

// *************************************************************
//		get/put_Key
// *************************************************************
void  CLinePattern::get_Key(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	*pVal = OLE2BSTR(_key);
	return ;
}
void  CLinePattern::put_Key(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	::SysFreeString(_key);
	_key = OLE2BSTR(newVal);
	return ;
}

// **************************************************************
//		CLinePattern::ErrorMessage()								      
// **************************************************************
void CLinePattern::ErrorMessage(long ErrorCode)
{
	_lastErrorCode = ErrorCode;
	CallbackHelper::ErrorMsg("LinePattern", _globalCallback, _key, ErrorMsg(_lastErrorCode));
}
#pragma endregion


// ***********************************************************
//		get_NumLines()
// ***********************************************************
void  CLinePattern::get_Count(int* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = _lines.size();
	return ;
}

// ***********************************************************
//		get_Line()
// ***********************************************************
void  CLinePattern::get_Line(int Index, CLineSegment** retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if(Index < 0 || Index >= (int)_lines.size())
	{
		ErrorMessage(tkINDEX_OUT_OF_BOUNDS);
		*retVal = NULL;
	}
	else
	{
		*retVal = _lines[Index];
	}
	return ;
}

// ***********************************************************
//		put_Line()
// ***********************************************************
void  CLinePattern::put_Line(int Index, CLineSegment* newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	if(Index < 0 || Index >= (int)_lines.size())
	{
		ErrorMessage(tkINDEX_OUT_OF_BOUNDS);
	}
	else
	{
		if (newVal)
		{
			/*ComHelper::SetRef(newVal, (IDispatch**)&_lines[Index], false);*/
		}
		else
		{
			ErrorMessage(tkUNEXPECTED_NULL_PARAMETER);
		}
	}
	return ;
}

#pragma region AddLine
// ***********************************************************
//		AddLine()
// ***********************************************************
void  CLinePattern::AddLine(OLE_COLOR color, float width, tkDashStyle style)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	VARIANT_BOOL vbretval;
	InsertLine(_lines.size(), color, width, style, &vbretval);
	return ;
}

// ***********************************************************
//		InsertLine()
// ***********************************************************
void  CLinePattern::InsertLine(int Index, OLE_COLOR color, float width, tkDashStyle style, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if (Index < 0 || Index > (int)_lines.size())
	{
		ErrorMessage(tkINDEX_OUT_OF_BOUNDS);
		*retVal = VARIANT_FALSE;
	}
	else
	{
		CLineSegment* segm = new CLineSegment();
		segm->put_Color(color);
		segm->put_LineWidth(width);
		segm->put_LineStyle(style);

		_lines.insert(_lines.begin() + Index, segm);
		*retVal = VARIANT_TRUE;
	}
	return ;
}
#pragma endregion

#pragma region AddMarker
// ***********************************************************
//		AddMarker()
// ***********************************************************
void  CLinePattern::AddMarker(tkDefaultPointSymbol marker, CLineSegment** retVal )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	InsertMarker(_lines.size(), marker, retVal);
	return ;
}

// ***********************************************************
//		InsertMarker()
// ***********************************************************
void  CLinePattern::InsertMarker(int Index, tkDefaultPointSymbol marker, CLineSegment** retVal )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if (Index < 0 || Index > (int)_lines.size())
	{
		ErrorMessage(tkINDEX_OUT_OF_BOUNDS);
		*retVal = NULL;
	}
	else
	{
		CLineSegment* segm = new CLineSegment();
		segm->put_Marker(marker);
		segm->put_LineType(lltMarker);
		delete segm;
		_lines.insert(_lines.begin() + Index, segm);
		*retVal = segm;
	}
	return ;
}
#pragma endregion

#pragma region RemoveClear
// ***********************************************************
//		RemoveLine()
// ***********************************************************
void  CLinePattern::RemoveItem(int Index, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if(Index < 0 && Index >= (int)_lines.size())
	{
		ErrorMessage(tkINDEX_OUT_OF_BOUNDS);
		*retVal = VARIANT_FALSE;
	}
	else
	{
		delete _lines[Index];
		_lines.erase(_lines.begin() + Index);
		*retVal = VARIANT_TRUE;
	}
	return ;
}

// ***********************************************************
//		ClearLines()
// ***********************************************************
void  CLinePattern::Clear()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	for (unsigned int i = 0; i < _lines.size(); i++)
	{
		if (_lines[i])
		{
			delete _lines[i];
			_lines[i] = NULL;
		}
	}

	_lines.clear();
	return ;
}
#pragma endregion

#pragma region Drawing

// *************************************************************
//		Draw()
// *************************************************************
void  CLinePattern::Draw(int** hdc, float x, float y, int clipWidth, int clipHeight, OLE_COLOR backColor, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if (!hdc)
	{
		ErrorMessage(tkUNEXPECTED_NULL_PARAMETER);
		return ;
	}

	CDC* dc = CDC::FromHandle((HDC)hdc);
	*retVal = this->DrawCore(dc, x, y, clipWidth, clipHeight, backColor);
	return ;
}

// *************************************************************
//		DrawVB()
// *************************************************************
void  CLinePattern::DrawVB(int hdc, float x, float y, int clipWidth, int clipHeight, OLE_COLOR backColor, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	CDC* dc = CDC::FromHandle((HDC)hdc);
	*retVal = this->DrawCore(dc, x, y, clipWidth, clipHeight, backColor);
	return ;
}

// *************************************************************
//		Draw()
// *************************************************************
VARIANT_BOOL CLinePattern::DrawCore(CDC* dc, float x, float y, int clipWidth, int clipHeight, OLE_COLOR backColor)
{
	if (!dc)
	{
		ErrorMessage(tkFAILED_TO_OBTAIN_DC);
		return VARIANT_FALSE;
	}
	
	Gdiplus::Bitmap bmp(clipWidth, clipHeight, PixelFormat32bppARGB);
	Gdiplus::Graphics g(&bmp);

	Gdiplus::Color clr(255 << 24 | BGR_TO_RGB(backColor));
	Gdiplus::SolidBrush brushBackground(clr);
	g.Clear(clr);
	
	BYTE transparency;
	this->get_Transparency(&transparency);

	Gdiplus::SmoothingMode mode;
	mode = g.GetSmoothingMode();

	VARIANT_BOOL vbretval;
	for (unsigned int i = 0; i < _lines.size(); i++)
	{
		tkLineType type;
		_lines[i]->get_LineType(&type);
		if (type == lltSimple)
		{
			g.SetSmoothingMode(Gdiplus::SmoothingModeDefault);
		}
		else
		{
			g.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
		}

		((CLineSegment*)_lines[i])->Draw(g, transparency, clipWidth, clipHeight, 0,0, &vbretval); //xOrigin, yOrigin, &vbretval);
	}

	g.SetSmoothingMode(mode);

	Gdiplus::Graphics gResult(dc->GetSafeHdc());
	gResult.DrawImage(&bmp, x, y);

	return VARIANT_TRUE;
}
#pragma endregion

// ****************************************************************
//		get_Transparency
// ****************************************************************
void  CLinePattern::get_Transparency (BYTE *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _transparency;
	return ;
}
void  CLinePattern::put_Transparency (BYTE newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	if (newVal < 0) newVal = 0;
	if (newVal > 255) newVal = 255;
	_transparency = newVal;	
	return ;
}

#pragma region "Serialization"
// ********************************************************
//     Serialize()
// ********************************************************
void  CLinePattern::Serialize(BSTR* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	CPLXMLNode* node = SerializeCore("LinePatternClass");
	Utility::SerializeAndDestroyXmlTree(node, retVal);
	return ;
}

// ********************************************************
//     SerializeCore()
// ********************************************************
CPLXMLNode* CLinePattern::SerializeCore(CString ElementName)
{
	USES_CONVERSION;
	CPLXMLNode* psTree = CPLCreateXMLNode( NULL, CXT_Element, ElementName);
	 
	CString str = OLE2CA(_key);
	Utility::CPLCreateXMLAttributeAndValue(psTree, "Key", CPLString().Printf(str));
	Utility::CPLCreateXMLAttributeAndValue(psTree, "Transparency", CPLString().Printf("%d", (int)_transparency));
	
	// segments
	if (_lines.size() > 0)
	{
		CPLXMLNode* psFields = CPLCreateXMLNode(psTree, CXT_Element, "LineSegments");
		if (psFields)
		{
			for (unsigned int i = 0; i < _lines.size(); i++)
			{
				CPLXMLNode* psNode = CPLCreateXMLNode(psFields, CXT_Element, "LineSegmentClass");

				// line type
				tkLineType lineType;
				_lines[i]->get_LineType(&lineType);
				Utility::CPLCreateXMLAttributeAndValue(psNode, "LineType", CPLString().Printf("%d", (int)lineType));
				
				// color
				OLE_COLOR color;
				_lines[i]->get_Color(&color);
				Utility::CPLCreateXMLAttributeAndValue(psNode, "Color", CPLString().Printf("%d", color));

				// marker outline color
				_lines[i]->get_MarkerOutlineColor(&color);
				Utility::CPLCreateXMLAttributeAndValue(psNode, "MarkerOutlineColor", CPLString().Printf("%d", color));

				// line width
				float val;
				_lines[i]->get_LineWidth(&val);
				Utility::CPLCreateXMLAttributeAndValue(psNode, "LineWidth", CPLString().Printf("%f", val));

				// marker size
				_lines[i]->get_MarkerSize(&val);
				Utility::CPLCreateXMLAttributeAndValue(psNode, "MarkerSize", CPLString().Printf("%f", val));

				// marker interval
				_lines[i]->get_MarkerInterval(&val);
				Utility::CPLCreateXMLAttributeAndValue(psNode, "MarkerInterval", CPLString().Printf("%f", val));

				// marker offset
				_lines[i]->get_MarkerOffset(&val);
				Utility::CPLCreateXMLAttributeAndValue(psNode, "MarkerOffset", CPLString().Printf("%f", val));

				// line style
				tkDashStyle lineStyle;
				_lines[i]->get_LineStyle(&lineStyle);
				Utility::CPLCreateXMLAttributeAndValue(psNode, "LineStyle", CPLString().Printf("%d", (int)lineStyle));

				// marker
				tkDefaultPointSymbol marker;
				_lines[i]->get_Marker(&marker);
				Utility::CPLCreateXMLAttributeAndValue(psNode, "Marker", CPLString().Printf("%d", (int)marker));

				// marker orientation
				tkLineLabelOrientation markerOrientation;
				_lines[i]->get_MarkerOrientation(&markerOrientation);
				Utility::CPLCreateXMLAttributeAndValue(psNode, "MarkerOrientation", CPLString().Printf("%d", (int)markerOrientation));

				// marker flip first
				VARIANT_BOOL markerFlipFirst;
				_lines[i]->get_MarkerFlipFirst(&markerFlipFirst);
				Utility::CPLCreateXMLAttributeAndValue(psNode, "MarkerFlipFirst", CPLString().Printf("%d", (int)markerFlipFirst));
			}
		}
	}
	return psTree;
}

// ********************************************************
//     DeserializeCore()
// ********************************************************
bool CLinePattern::DeserializeCore(CPLXMLNode* node)
{
	if (!node)
		return false;

	CString s;
	s = CPLGetXMLValue( node, "Key", NULL );
	if (s != "")
	{
		CComBSTR bstr(s);
		this->put_Key(bstr);
	}

	s = CPLGetXMLValue( node, "Transparency", NULL );
	if (s != "") _transparency = (unsigned char)atoi(s.GetString());
	
	// restoring segments
	this->Clear();

	node = CPLGetXMLNode(node, "LineSegments");
	if (node)
	{
		node = node->psChild;
		while (node)
		{
			if (strcmp(node->pszValue, "LineSegmentClass") == 0)
			{
				CLineSegment* segment = NULL;
				this->AddMarker(dpsDiamond, &segment);
				
				if (segment)
				{
					// line type
					tkLineType lineType;
					s = CPLGetXMLValue( node, "LineType", NULL );
					if (s != "") lineType = (tkLineType)atoi( s );
					segment->put_LineType(lineType);

					// color
					OLE_COLOR color = RGB(0,0,0);
					s = CPLGetXMLValue( node, "Color", NULL );
					if (s != "") color = (OLE_COLOR)atoi( s );
					segment->put_Color(color);

					// outline color
					color = RGB(0,0,0);
					s = CPLGetXMLValue( node, "MarkerOutlineColor", NULL );
					if (s != "") color = (OLE_COLOR)atoi( s );
					segment->put_MarkerOutlineColor(color);

					// line width
					float val = 0.0f;
					s = CPLGetXMLValue( node, "LineWidth", NULL );
					if (s != "") val = (float)Utility::atof_custom( s );
					segment->put_LineWidth(val);

					// marker size
					val = 0.0f;
					s = CPLGetXMLValue( node, "MarkerSize", NULL );
					if (s != "") val = (float)Utility::atof_custom( s );
					segment->put_MarkerSize(val);

					// marker interval
					val = 0.0f;
					s = CPLGetXMLValue( node, "MarkerInterval", NULL );
					if (s != "") val = (float)Utility::atof_custom( s );
					segment->put_MarkerInterval(val);

					// marker offset
					val = 0.0f;
					s = CPLGetXMLValue( node, "MarkerOffset", NULL );
					if (s != "") val = (float)Utility::atof_custom( s );
					segment->put_MarkerOffset(val);

					// line style
					tkDashStyle lineStyle = dsSolid;
					s = CPLGetXMLValue( node, "LineStyle", NULL );
					if (s != "") lineStyle = (tkDashStyle)atoi( s );
					segment->put_LineStyle(lineStyle);

					// marker
					tkDefaultPointSymbol marker = dpsDiamond;
					s = CPLGetXMLValue( node, "Marker", NULL );
					if (s != "") marker = (tkDefaultPointSymbol)atoi( s );
					segment->put_Marker(marker);

					// marker orientation
					tkLineLabelOrientation orientation = lorHorizontal;
					s = CPLGetXMLValue( node, "MarkerOrientation", NULL );
					if (s != "") orientation = (tkLineLabelOrientation)atoi( s );
					segment->put_MarkerOrientation(orientation);

					// marker flip first
					VARIANT_BOOL flipFirst = VARIANT_FALSE;
					s = CPLGetXMLValue( node, "MarkerFlipFirst", NULL );
					if (s != "") flipFirst = (VARIANT_BOOL)atoi( s );
					segment->put_MarkerFlipFirst(flipFirst);

					delete segment;
				}
			}
			node = node->psNext;
		} 
	}
	return true;
}

// ********************************************************
//     Deserialize()
// ********************************************************
void  CLinePattern::Deserialize(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;

	CString s = OLE2CA(newVal);
	CPLXMLNode* node = CPLParseXMLString(s.GetString());
	if (node)
	{
		CPLXMLNode* nodeLp = CPLGetXMLNode(node, "=LinePatternClass");
		if (nodeLp)
		{
			DeserializeCore(nodeLp);
		}
		CPLDestroyXMLNode(node);
	}
	return ;
}
#pragma endregion

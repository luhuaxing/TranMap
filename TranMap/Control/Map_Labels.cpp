// The old implementation of labels is stored here.
// The old properties are redirected to the new implementation where applicable.

#pragma once
#include "stdafx.h"
#include "Map.h"
#include "IndexSearching.h"
#include "LabelCategory.h"
#include "Labels.h"

// *************************************************************
//		LineSeparationFactor()
// *************************************************************
void CMapView::SetLineSeparationFactor(long sepFactor)
{
	_lineSeparationFactor = sepFactor;
}

long CMapView::GetLineSeparationFactor(void)
{
	//gReferenceCounter.WriteReport();
	// 0 is invalid, since it would result in no adjustments
    if (_lineSeparationFactor == 0) _lineSeparationFactor = 3;
	return _lineSeparationFactor;
}

// *************************************************************
//		GetOgrLayer()
// *************************************************************
COgrLayer* CMapView::GetOgrLayer(LONG LayerHandle)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (LayerHandle >= 0 && LayerHandle < (long)_allLayers.size())
	{
		Layer * layer = _allLayers[LayerHandle];
		COgrLayer* ogr = NULL;
		if (layer->QueryOgrLayer(&ogr))
			return ogr;
	}
	else
	{
		this->ErrorMessage(tkINVALID_LAYER_HANDLE);
	}
	return NULL;
}

// *************************************************************
//		GetShapefile()
// *************************************************************
CShapefile* CMapView::GetShapefile(LONG LayerHandle)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (LayerHandle >= 0 && LayerHandle < (long)_allLayers.size())
	{
		Layer * layer = _allLayers[LayerHandle];
		CShapefile* sf = NULL;
		if (layer->QueryShapefile(&sf))
			return sf;
	}
	return NULL;
}

void CMapView::SetShapefile(LONG LayerHandle, CShapefile* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	SetNotSupported();
}

// *************************************************************
//		GetImage()
// *************************************************************
CImageClass* CMapView::GetImage(LONG LayerHandle)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (LayerHandle >= 0 && LayerHandle < (long)_allLayers.size())
	{
		Layer * layer = _allLayers[LayerHandle];
		CImageClass* img = NULL;
		if (layer->QueryImage(&img))
		{
			return img;
		}
	}
	else
	{
		this->ErrorMessage(tkINVALID_LAYER_HANDLE);
	}
	return NULL;
}
void CMapView::SetImage(LONG LayerHandle, CImageClass* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	SetNotSupported();
}

// *************************************************************
//		GetLayerLabels()
// *************************************************************
CLabels* CMapView::GetLayerLabels(LONG LayerHandle)
{
	if (LayerHandle >= 0 && LayerHandle < (long)_allLayers.size())
	{
		Layer * layer = _allLayers[LayerHandle];
		return layer->get_Labels();
	}
	else
	{
		this->ErrorMessage(tkINVALID_LAYER_HANDLE);
	}
	return NULL;
}

// *************************************************************
//		SetLayerLabels()
// *************************************************************
void CMapView::SetLayerLabels(LONG LayerHandle, CLabels* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
    GetDocument()->SetModifiedFlag();
}

// *************************************************************
//		GetLayerLabelsVisible()
// *************************************************************
BOOL CMapView::GetLayerLabelsVisible(long LayerHandle)
{
	CLabels* labels = GetLayerLabels(LayerHandle);
	if (labels)
	{
		VARIANT_BOOL visible;
		labels->get_Visible(&visible);
		delete labels;
		return visible == VARIANT_TRUE?TRUE:FALSE;
	}
	else
		return FALSE;
}

// *************************************************************
//		SetLayerLabelsVisible()
// *************************************************************
void CMapView::SetLayerLabelsVisible(long LayerHandle, BOOL bNewValue)
{
	CLabels* labels = GetLayerLabels(LayerHandle);
	if (labels)
	{
		VARIANT_BOOL visible = (bNewValue == TRUE)?VARIANT_TRUE:VARIANT_FALSE;
		labels->put_Visible(visible);
		delete labels;
	}
}

// *************************************************************
//		GetLayerLabelsShadow()
// *************************************************************
BOOL CMapView::GetLayerLabelsShadow(long LayerHandle)
{
	CLabels* labels = GetLayerLabels(LayerHandle);
	if (labels)
	{
		VARIANT_BOOL visible;
		labels->get_HaloVisible(&visible);
		delete labels;
		return visible == VARIANT_TRUE?TRUE:FALSE;
	}
	else
		return FALSE;
}

// *************************************************************
//		SetLayerLabelsShadow()
// *************************************************************
void CMapView::SetLayerLabelsShadow(long LayerHandle, BOOL newValue)
{
	CLabels* labels = GetLayerLabels(LayerHandle);
	if (labels)
	{
		VARIANT_BOOL visible = (newValue == TRUE)?VARIANT_TRUE:VARIANT_FALSE;
		labels->put_HaloVisible(visible);
		//labels->Release();
	}
}

// **********************************************************
//		GetLayerLabelsScale()
// **********************************************************
BOOL CMapView::GetLayerLabelsScale(long LayerHandle)
{
	CLabels* labels = GetLayerLabels(LayerHandle);
	if (labels)
	{
		VARIANT_BOOL scale;
		labels->get_ScaleLabels(&scale);
		delete labels;
		return scale == VARIANT_TRUE?TRUE:FALSE;
	}
	else
		return FALSE;
}

// **********************************************************
//		SetLayerLabelsScale()
// **********************************************************
void CMapView::SetLayerLabelsScale(long LayerHandle, BOOL newValue)
{
	CLabels* labels = GetLayerLabels(LayerHandle);
	if (labels)
	{
		VARIANT_BOOL scale = (newValue == TRUE)?VARIANT_TRUE:VARIANT_FALSE;
		labels->put_ScaleLabels(scale);
		delete labels;
	}
}

// **********************************************************
//		GetLayerLabelsOffset()
// **********************************************************
long CMapView::GetLayerLabelsOffset(long LayerHandle)
{
	CLabels* labels = GetLayerLabels(LayerHandle);
	if (labels)
	{
		double offset;
		labels->get_OffsetX(&offset);
		delete labels;
		return (long)offset;
	}
	else
		return 0;
}

// **********************************************************
//		GetLayerLabelsOffset()
// **********************************************************
void CMapView::SetLayerLabelsOffset(long LayerHandle, long Offset)
{
	CLabels* labels = GetLayerLabels(LayerHandle);
	if (labels)
	{
		double offset = (double)Offset;
		labels->put_OffsetX(offset);
		delete labels;
	}
}

// **********************************************************
//		GetLayerLabelsShadowColor()
// **********************************************************
OLE_COLOR CMapView::GetLayerLabelsShadowColor(long LayerHandle)
{
	CLabels* labels = GetLayerLabels(LayerHandle);
	if (labels)
	{
		OLE_COLOR color;
		labels->get_HaloColor(&color);
		delete labels;
		return color;
	}
	else
		return 0;
}

// **********************************************************
//		SetLayerLabelsShadowColor()
// **********************************************************
void CMapView::SetLayerLabelsShadowColor(long LayerHandle, OLE_COLOR color)
{
	CLabels* labels = GetLayerLabels(LayerHandle);
	if (labels)
	{
		labels->put_HaloColor(color);
		delete labels;
	}
}

// **********************************************************
//		GetMultilineLabeling()
// **********************************************************
bool CMapView::GetMultilineLabeling()
{
	return _multilineLabeling?true:false;
}

// **********************************************************
//		SetMultilineLabeling()
// **********************************************************
void CMapView::SetMultilineLabeling(bool Value)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	_multilineLabeling = Value;		// we shall save the value all the same
}

// **********************************************************
//		GetUseLabelCollision()
// **********************************************************
BOOL CMapView::GetUseLabelCollision(long LayerHandle)
{
	CLabels* labels = GetLayerLabels(LayerHandle);
	if (labels)
	{
		VARIANT_BOOL value;
		labels->get_AvoidCollisions(&value);
		delete labels;
		return (value == VARIANT_TRUE)?TRUE:FALSE;
	}
	else
		return FALSE;
}

// **********************************************************
//		SetUseLabelCollision()
// **********************************************************
void CMapView::SetUseLabelCollision(long LayerHandle, BOOL value)
{
	CLabels* labels = GetLayerLabels(LayerHandle);
	if (labels)
	{
		VARIANT_BOOL avoid = (value == TRUE)?VARIANT_TRUE:VARIANT_FALSE;
		labels->put_AvoidCollisions(avoid);
		delete labels;
	}
}

// **********************************************************
//		LayerFont()
// **********************************************************
void CMapView::LayerFont(long LayerHandle, LPCTSTR FontName, long FontSize)
{
	CLabels* labels = GetLayerLabels(LayerHandle);
	if (labels)
	{
		CComBSTR name(FontName);
		labels->put_FontName(name);
		labels->put_FontSize(FontSize);
		delete labels;
	}
}

// **********************************************************
//		LayerFontEx()
// **********************************************************
void CMapView::LayerFontEx(long LayerHandle, LPCTSTR FontName, long FontSize, BOOL isBold, BOOL isItalic, BOOL isUnderline)
{
	CLabels* labels = GetLayerLabels(LayerHandle);
	if (labels)
	{
		CComBSTR name(FontName);
		labels->put_FontName(name);
		labels->put_FontSize(FontSize);
		
		VARIANT_BOOL bold = (isBold == TRUE)? VARIANT_TRUE: VARIANT_FALSE;
		VARIANT_BOOL italic = (isItalic == TRUE)? VARIANT_TRUE: VARIANT_FALSE;
		VARIANT_BOOL underline = (isUnderline == TRUE)? VARIANT_TRUE: VARIANT_FALSE;

		labels->put_FontBold(bold);
		labels->put_FontItalic(italic);
		labels->put_FontUnderline(underline);
		
		delete labels;
	}
}

// **********************************************************
//		LabelColor()
// **********************************************************
void CMapView::LabelColor(LONG LayerHandle, OLE_COLOR LabelFontColor)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CLabels* labels = GetLayerLabels(LayerHandle);
	if (labels)
	{
		USES_CONVERSION;
		labels->put_FontColor(LabelFontColor);
		delete labels;
	}
}

// **********************************************************
//		GetLayerStandardViewWidth()
// **********************************************************
void CMapView::GetLayerStandardViewWidth(long LayerHandle, double * Width)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
	*Width = 0.0;
}

// **********************************************************
//		SetLayerStandardViewWidth()
// **********************************************************
void CMapView::SetLayerStandardViewWidth(long LayerHandle, double Width)
{
	ErrorMessage(tkPROPERTY_DEPRECATED);
}
#pragma endregion

#pragma region Methods
// **********************************************************
//		AddLabel()
// **********************************************************
void CMapView::AddLabel(long LayerHandle, LPCTSTR Text, OLE_COLOR Color, double x, double y, short hJustification)
{
	CLabels* labels = GetLayerLabels(LayerHandle);
	if (labels)
	{
		CComBSTR s(Text);
		labels->AddLabel(s, x, y,0.0);
	}
}

// **********************************************************
//		AddLabelEx()
// **********************************************************
void CMapView::AddLabelEx(long LayerHandle, LPCTSTR Text, OLE_COLOR Color, double x, double y, short hJustification, double Rotation)
{
	CLabels* labels = GetLayerLabels(LayerHandle);
	if (labels)
	{
		CComBSTR s(Text);
		labels->AddLabel(s, x, y, Rotation);
	}
}

// **********************************************************
//		ClearLabels()
// **********************************************************
void CMapView::ClearLabels(long LayerHandle)
{
	CLabels* labels = GetLayerLabels(LayerHandle);
	if (labels)
	{
		labels->Clear();
		delete labels;
	}
}


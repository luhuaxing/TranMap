#pragma once
#include "LabelOptions.h"
#include "Extents.h"

// CLabelCategory
class  CLabelCategory
{
public:
	CLabelCategory()
	{
		_name = SysAllocString(L"");
		_expression = SysAllocString(L"");
		VariantInit(&_minValue);
		VariantInit(&_maxValue);
		_priority = 0;
		_enabled = VARIANT_TRUE;
		m_value.vt = VT_EMPTY;
		gReferenceCounter.AddRef(tkInterface::idLabelCategory);
	}
	
	~CLabelCategory()
	{
		::SysFreeString(_name);
		::SysFreeString(_expression);
		VariantClear(&_minValue);
		VariantClear(&_maxValue);
		gReferenceCounter.Release(tkInterface::idLabelCategory);
	}


public:
	void get_Name(BSTR* retval);
	void put_Name(BSTR newVal);

	void get_Expression(BSTR* retval);
	void put_Expression(BSTR newVal);

	void get_MinValue(VARIANT* pVal);
	void put_MinValue(VARIANT newVal);

	void get_MaxValue(VARIANT* pVal);
	void put_MaxValue(VARIANT newVal);

	void get_Priority(LONG* retval)							{*retval = _priority;			return ;};
	void put_Priority(LONG newVal)							{_priority = newVal;			return ;};
	
	// ---------------------------------------------------------------------------------
	// Properties common for CLabels and CLabelCategory
	// it's easier to port the options in separate class to avoid duplication;
	// but such disposition upon my opinion ensure more readable code in the client part
	// Also it's possible to treat CLabels basic options like Category with index 0
	// but again the code outside will be easier if we use just Labels.FontColor rather than 
	// Labels.Category[0].FontColor or Labels.Options.FontColor, therefore such disposition
	// CLabelOptions structure is used to hold the options, so delcarations can be just copied
	// between CLabels and CLabelCategory, the names of properties should be the same in both classes
	// ---------------------------------------------------------------------------------
	void get_Visible(VARIANT_BOOL* retval)					{*retval = _options.visible;			return ;};
	void put_Visible(VARIANT_BOOL newVal)						{_options.visible = newVal?true:false;			return ;};		
	
	// position
	void get_OffsetX(double* retval)							{*retval = _options.offsetX;			return ;};
	void put_OffsetX(double newVal)							{_options.offsetX = newVal;			return ;};
	void get_OffsetY(double* retval)							{*retval = _options.offsetY;			return ;};
	void put_OffsetY(double newVal)							{_options.offsetY = newVal;			return ;};
	void get_Alignment(tkLabelAlignment* retval)				{*retval = _options.alignment;			return ;};
	void put_Alignment(tkLabelAlignment newVal)				{_options.alignment = newVal;			return ;};
	void get_LineOrientation(tkLineLabelOrientation* retval)	{*retval = _options.lineOrientation;	return ;};		
	void put_LineOrientation(tkLineLabelOrientation newVal)	{_options.lineOrientation = newVal;	return ;};
	
	// font
	void get_FontName(BSTR* retval);
	void put_FontName(BSTR newVal);
	void get_FontSize(LONG* retval)							{*retval = _options.fontSize;			return ;};
	void put_FontSize(LONG newVal)							{_options.fontSize = newVal;			return ;};
	
	void get_FontItalic(VARIANT_BOOL* retval);
	void put_FontItalic(VARIANT_BOOL newVal);
	void get_FontBold(VARIANT_BOOL* retval);
	void put_FontBold(VARIANT_BOOL newVal);
	void get_FontUnderline(VARIANT_BOOL* retval);
	void put_FontUnderline(VARIANT_BOOL newVal);
	void get_FontStrikeOut(VARIANT_BOOL* retval);
	void put_FontStrikeOut(VARIANT_BOOL newVal);
	
	void get_FontColor(OLE_COLOR* retval)						{*retval = _options.fontColor;			return ;};
	void put_FontColor(OLE_COLOR newVal)						{_options.fontColor = newVal;			return ;};
	void get_FontColor2(OLE_COLOR* retval)					{*retval = _options.fontColor2;		return ;};
	void put_FontColor2(OLE_COLOR newVal)						{_options.fontColor2 = newVal;			return ;};
	void get_FontGradientMode(tkLinearGradientMode* retval)	{*retval = _options.fontGradientMode;	return ;};
	void put_FontGradientMode(tkLinearGradientMode newVal)	{_options.fontGradientMode = newVal;	return ;};

	void get_FontTransparency(LONG* retval);
	void put_FontTransparency(LONG newVal);
	
	// outlines
	void get_FontOutlineVisible(VARIANT_BOOL* retval)			{*retval = _options.fontOutlineVisible;			return ;};
	void put_FontOutlineVisible(VARIANT_BOOL newVal)			{_options.fontOutlineVisible = newVal?true:false;	return ;};		
	void get_ShadowVisible(VARIANT_BOOL* retval)				{*retval = _options.shadowVisible;					return ;};
	void put_ShadowVisible(VARIANT_BOOL newVal)				{_options.shadowVisible = newVal?true:false;		return ;};		
	void get_HaloVisible(VARIANT_BOOL* retval)				{*retval = _options.haloVisible;					return ;};
	void put_HaloVisible(VARIANT_BOOL newVal)					{_options.haloVisible = newVal?true:false;			return ;};		
	
	void get_FontOutlineColor(OLE_COLOR* retval)				{*retval = _options.fontOutlineColor;	return ;};
	void put_FontOutlineColor(OLE_COLOR newVal)				{_options.fontOutlineColor = newVal;	return ;};
	void get_ShadowColor(OLE_COLOR* retval)					{*retval = _options.shadowColor;		return ;};
	void put_ShadowColor(OLE_COLOR newVal)					{_options.shadowColor = newVal;		return ;};
	void get_HaloColor(OLE_COLOR* retval)						{*retval = _options.haloColor;			return ;};
	void put_HaloColor(OLE_COLOR newVal)						{_options.haloColor = newVal;			return ;};

	void get_FontOutlineWidth(LONG* retval)					{*retval = _options.fontOutlineWidth;	return ;};
	void put_FontOutlineWidth(LONG newVal)					{_options.fontOutlineWidth = newVal;	return ;};
	void get_ShadowOffsetX(LONG* retval)						{*retval = _options.shadowOffsetX;		return ;};
	void put_ShadowOffsetX(LONG newVal)						{_options.shadowOffsetX = newVal;		return ;};
	void get_ShadowOffsetY(LONG* retval)						{*retval = _options.shadowOffsetY;		return ;};
	void put_ShadowOffsetY(LONG newVal)						{_options.shadowOffsetY = newVal;		return ;};
	
	// haloSize - in 1/16 of font height
	void get_HaloSize(LONG* retval)							{*retval = _options.haloSize;			return ;};
	void put_HaloSize(LONG newVal)							{_options.haloSize = newVal;			return ;};
	
	// frame
	void get_FrameType(tkLabelFrameType* retval)				{*retval = _options.frameType;			return ;};		
	void put_FrameType(tkLabelFrameType newVal)				{_options.frameType = newVal;			return ;};
	void get_FrameOutlineColor(OLE_COLOR* retval)				{*retval = _options.frameOutlineColor;	return ;};		
	void put_FrameOutlineColor(OLE_COLOR newVal)				{_options.frameOutlineColor = newVal;	return ;};
	void get_FrameBackColor(OLE_COLOR* retval)				{*retval = _options.frameBackColor;	return ;};		
	void put_FrameBackColor(OLE_COLOR newVal)					{_options.frameBackColor = newVal;		return ;};
	void get_FrameBackColor2(OLE_COLOR* retval)				{*retval = _options.frameBackColor2;	return ;};		
	void put_FrameBackColor2(OLE_COLOR newVal)				{_options.frameBackColor2 = newVal;	return ;};
	void get_FrameGradientMode(tkLinearGradientMode* retval)	{*retval = _options.frameGradientMode;	return ;};
	void put_FrameGradientMode(tkLinearGradientMode newVal)	{_options.frameGradientMode = newVal;	return ;};

	void get_FrameOutlineStyle(tkDashStyle* retval)			{*retval = _options.frameOutlineStyle;	return ;};		
	void put_FrameOutlineStyle(tkDashStyle newVal)			{_options.frameOutlineStyle = newVal;	return ;};
	void get_FrameOutlineWidth(LONG* retval)					{*retval = _options.frameOutlineWidth;	return ;};		
	void put_FrameOutlineWidth(LONG newVal)					{if (newVal >= 1) _options.frameOutlineWidth = newVal;	return ;};
	
	void get_FramePaddingX(LONG* retval)						{*retval = _options.framePaddingX;		return ;};		
	void put_FramePaddingX(LONG newVal)						{_options.framePaddingX = newVal;		return ;};
	void get_FramePaddingY(LONG* retval)						{*retval = _options.framePaddingY;		return ;};		
	void put_FramePaddingY(LONG newVal)						{_options.framePaddingY = newVal;		return ;};
	
	void get_FrameTransparency(long* retval);	
	void put_FrameTransparency(long newVal);

	void get_InboxAlignment(tkLabelAlignment* retval)			{*retval = _options.inboxAlignment;	return ;};		
	void put_InboxAlignment(tkLabelAlignment newVal)			{_options.inboxAlignment = newVal;		return ;};

	void get_FrameVisible(VARIANT_BOOL* retval)				{*retval = _options.frameVisible;	return ;};		
	void put_FrameVisible(VARIANT_BOOL newVal)				{_options.frameVisible = newVal;	return ;};

	// ------------------------------------------------------------------
	//	end of properties common for CLabels and CLabelCategory classes
	// ------------------------------------------------------------------
	
	void get_Enabled(VARIANT_BOOL* retval)			{*retval = _enabled;	return ;};		
	void put_Enabled(VARIANT_BOOL newVal)				{_enabled = newVal;	return ;};

	void Serialize(BSTR* retVal);
	void Deserialize(BSTR newVal);

private:
	BSTR _name;
	BSTR _expression;
	VARIANT _minValue;
	VARIANT _maxValue;
	VARIANT_BOOL _enabled;		// doesn't influence the drawing; is needed for symbology plug-in
	long _priority;
	CLabelOptions _options;

public:
	CLabelOptions* get_LabelOptions();
	void put_LabelOptions(CLabelOptions* newVal);
	CComVariant m_value;
	void DeserializeFromNode(CPLXMLNode* node);
	CPLXMLNode* SerializeCore(CString ElementName);
};




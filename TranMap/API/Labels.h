#pragma once
#include "LabelClass.h"
#include "LabelOptions.h"
#include "LabelCategory.h"
#include "ColorScheme.h"
#include "Extents.h"

extern GlobalSettingsInfo m_globalSettings;

// CLabels
class  CLabels
{
public:
	CLabels()
	{
		_floatNumberFormat = m_globalSettings.floatNumberFormat;
		_shapefile = NULL;
		_synchronized = VARIANT_FALSE;
		USES_CONVERSION;
		_key = SysAllocString(L"");
		_expression = SysAllocString(L"");
		_globalCallback = NULL;
		_lastErrorCode = tkNO_ERROR;
		_scale = false;
		_verticalPosition = vpAboveAllLayers;
		_basicScale = 0.0;
		_maxVisibleScale = 100000000.0;
		_minVisibleScale = 0.0;
		_collisionBuffer = 0;

		_dynamicVisibility = VARIANT_FALSE;
		_avoidCollisions = (m_globalSettings.labelsCollisionMode != tkCollisionMode::AllowCollisions);
		_useWidthLimits = VARIANT_FALSE;
		_removeDuplicates = VARIANT_FALSE;
		_gdiPlus = VARIANT_TRUE;

		_classificationField = -1;
		_sourceField = -1;

		_minDrawingSize = 1;	// turn off
		_autoOffset = VARIANT_TRUE;

		_savingMode = modeXML;
		_positioning = lpNone;

		_textRenderingHint = AntiAliasGridFit;

		gReferenceCounter.AddRef(tkInterface::idLabels);
	}
	~CLabels()
	{
		Clear();
		ClearCategories();
		::SysFreeString(_key);
		::SysFreeString(_expression);		
		_shapefile = NULL;
		gReferenceCounter.Release(tkInterface::idLabels);
	}


public:
	void get_Key(/*[out, retval]*/ BSTR *pVal);
	void put_Key(/*[in]*/ BSTR newVal);
	void get_GlobalCallback(/*[out, retval]*/ ICallback * *pVal);
	void put_GlobalCallback(/*[in]*/ ICallback * newVal);
	void get_ErrorMsg(/*[in]*/ long ErrorCode, /*[out, retval]*/ BSTR *pVal);
	void get_LastErrorCode(/*[out, retval]*/ long *pVal);
	
	
	void AddLabel(BSTR Text, double x, double y, double Rotation, long Category = -1);
	void InsertLabel(long Index, BSTR Text, double x, double y, double Rotation, long Category, VARIANT_BOOL* retVal);
	void RemoveLabel(long Index, VARIANT_BOOL* vbretval);
	
	void AddPart(long Index, BSTR Text, double x, double y, double Rotation, long Category = -1);
	void InsertPart(long Index, long Part, BSTR Text, double x, double y, double Rotation, long Category, VARIANT_BOOL* retVal);
	void RemovePart(long Index, long Part, VARIANT_BOOL* vbretval);

	void AddCategory(BSTR Name, CLabelCategory** retVal);
	void InsertCategory(long Index, BSTR Name, CLabelCategory** retVal);
	void RemoveCategory(long Index, VARIANT_BOOL* vbretval);

	void Clear();
	void ClearCategories();

	// managing labels
	void get_Count(/*[out, retval]*/long* pVal)				{*pVal = _labels.size();	return ;};
	void get_NumParts(/*[in]*/long Index, /*[out, retval]*/long* pVal);
	void get_NumCategories(/*[out, retval]*/long* pVal);
	
	void get_Label(long Index, long Part, CLabelClass** pVal);
	//void put_Label(long Index, long Part, CLabel* newVal);
	
	void get_Category(long Index, CLabelCategory** retval);
	void put_Category(long Index, CLabelCategory* newVal);

	// selection
	void Select(CExtents* BoundingBox, long Tolerance, SelectMode SelectMode, VARIANT* LabelIndices, VARIANT* PartIndices, VARIANT_BOOL* retval);

	// ------------------------------------------------------
	// Class-specific properties
	// ------------------------------------------------------
	void get_UseGdiPlus(VARIANT_BOOL* retval)						{*retval = _gdiPlus;				return ;};		
	void put_UseGdiPlus(VARIANT_BOOL newVal)						{_gdiPlus = newVal;				return ;};
	
	void get_Synchronized(VARIANT_BOOL* retval);
	void put_Synchronized(VARIANT_BOOL newVal);
	
	void get_ScaleLabels(VARIANT_BOOL* retval)					{*retval = _scale;					return ;};
	void put_ScaleLabels(VARIANT_BOOL newVal)						{_scale = newVal?true:false;		return ;};

	void get_VerticalPosition(tkVerticalPosition* retval)	{*retval = _verticalPosition;		return ;};	
	void put_VerticalPosition(tkVerticalPosition newVal)		{_verticalPosition = newVal;		return ;};

	void get_BasicScale(double* retval)							{*retval = _basicScale;			return ;};	
	void put_BasicScale(double newVal)							{_basicScale = newVal;				return ;};

	void get_MaxVisibleScale(double* retval)						{*retval = _maxVisibleScale;		return ;};		
	void put_MaxVisibleScale(double newVal)						{_maxVisibleScale = newVal;		return ;};
	
	void get_MinVisibleScale(double* retval)						{*retval = _minVisibleScale;		return ;};		
	void put_MinVisibleScale(double newVal)						{_minVisibleScale = newVal;		return ;};

	void get_DynamicVisibility(VARIANT_BOOL* retval)				{*retval = _dynamicVisibility;		return ;};		
	void put_DynamicVisibility(VARIANT_BOOL newVal)				{_dynamicVisibility = newVal;		return ;};

	void get_AvoidCollisions(VARIANT_BOOL* retval)				{*retval = _avoidCollisions;		return ;};		
	void put_AvoidCollisions(VARIANT_BOOL newVal)					{_avoidCollisions = newVal;		return ;};

	void get_CollisionBuffer(long* retval)						{*retval = _collisionBuffer;		return ;};		
	void put_CollisionBuffer(long newVal)							{_collisionBuffer = newVal;		return ;};
	
	// not implemented
	void get_UseWidthLimits(VARIANT_BOOL* retval)					{*retval = _useWidthLimits;		return ;};		
	void put_UseWidthLimits(VARIANT_BOOL newVal)					{_useWidthLimits = newVal;			return ;};
	
	// not implemented
	void get_RemoveDuplicates(VARIANT_BOOL* retval)				{*retval = _removeDuplicates;		return ;};		
	void put_RemoveDuplicates(VARIANT_BOOL newVal)				{_removeDuplicates = newVal;		return ;};
	
	// ---------------------------------------------------------------------------------
	// Properties common for CLabels and CLabelCategory
	// it's easier to port the options in separate class to avoid duplication;
	// but such disposition upon my opinion ensure more readable code in the client part
	// Also it's possible to treat CLabels basic options like Category with index 0
	// but again the code outside will be easier if we use just Labels.FontColor rather than 
	// Labels.Category[0].FontColor or Labels.Options.FontColor, therefore such disposition
	// CLabelOptions structure is used to hold the options, so declarations can be just copied
	// between CLabels and CLabelCategory, the names of properties should be the same in both classes
	// ---------------------------------------------------------------------------------
	void get_Visible(VARIANT_BOOL* retval)					{*retval = _options.visible;			return ;};
	void put_Visible(VARIANT_BOOL newVal)						{_options.visible = newVal?true:false;			return ;};		
	
	// position
	void get_OffsetX(DOUBLE* retval)							{*retval = _options.offsetX;			return ;};
	void put_OffsetX(DOUBLE newVal)							{_options.offsetX = newVal;			return ;};
	void get_OffsetY(DOUBLE* retval)							{*retval = _options.offsetY;			return ;};
	void put_OffsetY(DOUBLE newVal)							{_options.offsetY = newVal;			return ;};
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
	void get_FontOutlineVisible(VARIANT_BOOL* retval)			{*retval = _options.fontOutlineVisible;				return ;};
	void put_FontOutlineVisible(VARIANT_BOOL newVal)			{_options.fontOutlineVisible = newVal?true:false;		return ;};		
	void get_ShadowVisible(VARIANT_BOOL* retval)				{*retval = _options.shadowVisible;						return ;};
	void put_ShadowVisible(VARIANT_BOOL newVal)				{_options.shadowVisible = newVal?true:false;			return ;};		
	void get_HaloVisible(VARIANT_BOOL* retval)				{*retval = _options.haloVisible;						return ;};
	void put_HaloVisible(VARIANT_BOOL newVal)					{_options.haloVisible = newVal?true:false;				return ;};		
	
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
	void put_FrameOutlineWidth(LONG newVal)					{if (newVal >= 1)_options.frameOutlineWidth = newVal;	return ;};
	
	void get_FramePaddingX(LONG* retval)						{*retval = _options.framePaddingX;		return ;};		
	void put_FramePaddingX(LONG newVal)						{_options.framePaddingX = newVal;		return ;};
	void get_FramePaddingY(LONG* retval)						{*retval = _options.framePaddingY;		return ;};		
	void put_FramePaddingY(LONG newVal)						{_options.framePaddingY = newVal;		return ;};
	
	void get_FrameTransparency(long* retval);	
	void put_FrameTransparency(long newVal);

	void get_InboxAlignment(tkLabelAlignment* retval)			{*retval = _options.inboxAlignment;	return ;};		
	void put_InboxAlignment(tkLabelAlignment newVal)			{_options.inboxAlignment = newVal;		return ;};
	// ------------------------------------------------------------------
	//	end of properties common for CLabels and CLabelCategory classes
	// ------------------------------------------------------------------
	void GenerateCategories(long FieldIndex, tkClassificationType ClassificationType, long numClasses, VARIANT_BOOL* retVal);
	
	void ApplyCategories();
	
	void get_ClassificationField(long* FieldIndex);
	void put_ClassificationField(long FieldIndex);

	void get_Options(CLabelCategory** retVal);
	void put_Options(CLabelCategory* newVal);

	void ApplyColorScheme(tkColorSchemeType Type, CColorScheme* ColorScheme);
	void ApplyColorScheme2(tkColorSchemeType Type, CColorScheme* ColorScheme, tkLabelElements Element);
	void ApplyColorScheme3(tkColorSchemeType Type, CColorScheme* ColorScheme, 
											 tkLabelElements Element, long CategoryStartIndex, long CategoryEndIndex);

	void get_FrameVisible(VARIANT_BOOL* retVal)				{*retVal = _options.frameVisible;	return ;};
	void put_FrameVisible(VARIANT_BOOL newVal)				{_options.frameVisible = newVal;	return ;};
	
	void get_VisibilityExpression(BSTR* retval);
	void put_VisibilityExpression(BSTR newVal);

	void get_MinDrawingSize(LONG* retval);
	void put_MinDrawingSize(LONG newVal);
	
	void MoveCategoryUp(long Index, VARIANT_BOOL* retval);
	void MoveCategoryDown(long Index, VARIANT_BOOL* retval);

	void get_AutoOffset(VARIANT_BOOL* retval);
	void put_AutoOffset(VARIANT_BOOL newVal);

	void get_Expression(BSTR* retVal);
	void put_Expression(BSTR newVal);
	
	// serialization
	void Serialize(BSTR* retVal);
	void Deserialize(BSTR newVal);

	void SaveToDbf(VARIANT_BOOL saveText, VARIANT_BOOL saveCategory, VARIANT_BOOL* retVal);
	void SaveToDbf2(BSTR xField, BSTR yField, BSTR angleField, BSTR textField, BSTR categoryField, 
						 VARIANT_BOOL saveText, VARIANT_BOOL saveCategory, VARIANT_BOOL* retVal);

	void LoadFromDbf(VARIANT_BOOL loadText, VARIANT_BOOL loadCategory, VARIANT_BOOL* retVal);
	void LoadFromDbf2(BSTR xField, BSTR yField, BSTR angleField, BSTR textField, BSTR categoryField, 
						   VARIANT_BOOL loadText, VARIANT_BOOL loadCategory, VARIANT_BOOL* retVal);

	void SaveToXML(BSTR filename, VARIANT_BOOL* retVal);
	void LoadFromXML(BSTR filename, VARIANT_BOOL* retVal);
	void Generate(BSTR Expression, tkLabelPositioning Method, VARIANT_BOOL LargestPartOnly, long* Count);

	void get_SavingMode(tkSavingMode* retVal);
	void put_SavingMode(tkSavingMode newVal);

	void get_Positioning(tkLabelPositioning* pVal);
	void put_Positioning(tkLabelPositioning newVal);

	void get_TextRenderingHint(tkTextRenderingHint* pVal);
	void put_TextRenderingHint(tkTextRenderingHint newVal);

	void get_FloatNumberFormat(BSTR* pVal);
	void put_FloatNumberFormat(BSTR newVal);

	void ForceRecalculateExpression();

private:
	int _sourceField;

	tkLabelPositioning _positioning;
	
	tkSavingMode _savingMode;
	
	// expression to generate label text
	CString _labelExpression;
	CString _floatNumberFormat;

	// the minimal size of shape to label
	long _minDrawingSize;		
	
	// visibility expression
	BSTR _expression;

	long _classificationField;

	CShapefile* _shapefile;

	// inner vector for a given index should exist even if there are no actual labels (parts) in it
	std::vector<std::vector<CLabelInfo*>*> _labels;
	std::vector<CLabelCategory*> _categories;

	CLabelOptions _options;
	
	BSTR _key;
	long _lastErrorCode;
	ICallback * _globalCallback;
	
	tkVerticalPosition _verticalPosition;

	double		_basicScale;
	double		_maxVisibleScale;
	double		_minVisibleScale;
	long		_collisionBuffer;
	bool		_scale;
	
	VARIANT_BOOL _autoOffset;
	VARIANT_BOOL _dynamicVisibility;
	VARIANT_BOOL _avoidCollisions;
	VARIANT_BOOL _useWidthLimits;
	VARIANT_BOOL _removeDuplicates;
	VARIANT_BOOL _gdiPlus;

	tkTextRenderingHint _textRenderingHint;
	VARIANT_BOOL _synchronized;

private:
	inline void ErrorMessage(long ErrorCode);
	bool GetMinMaxCategoryValue(double& globalMax, double& globalMin);

public:
	void ClearLabelFrames();
	void SaveSourceField(int fieldIndex) {	_sourceField = fieldIndex;	}
	bool HasRotation();

	bool DeserializeCore(CPLXMLNode* node);
	CPLXMLNode* SerializeCore(CString ElementName);

	std::vector<std::vector<CLabelInfo*>*>* get_LabelData();
	CLabelOptions* get_LabelOptions();

	void put_ParentShapefile(CShapefile* newVal);
	CShapefile* get_ParentShapefile();
	void RefreshExpressions();
	bool LabelsSynchronized();

	void ApplyExpression_(long CategoryIndex);

	CPLXMLNode* SerializeLabelData(CString ElementName, bool& saveRotation, bool& saveText);
	bool DeserializeLabelData(CPLXMLNode* node, bool loadRotation, bool loadText);

	bool GenerateEmptyLabels();
	void AddEmptyLabel();

	void LoadLblOptions(CPLXMLNode* node);
};
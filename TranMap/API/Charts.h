#pragma once
#include "ChartField.h"
#include "Chart.h"

// underlying structure
struct ChartOptions
{
	bool useVariableRadius;
	bool use3Dmode;

	double rotation;
	double thickness;
	double tilt;
	
	long transparency;
	long barWidth;
	long barHeight;
	long radius;
	long normalizationField;
	long sizeField;
	long radius2;

	OLE_COLOR lineColor;
	VARIANT_BOOL visible;
	VARIANT_BOOL avoidCollisions;
	tkChartType chartType;
	tkVerticalPosition verticalPosition;
	
	BSTR valuesFontName;
	VARIANT_BOOL valuesVisible;
	VARIANT_BOOL valuesFrameVisible;
	VARIANT_BOOL valuesFontItalic;
	VARIANT_BOOL valuesFontBold;
	OLE_COLOR valuesFontColor;
	OLE_COLOR valuesFrameColor;
	long valuesFontSize;
	tkChartValuesStyle valuesStyle;

	ChartOptions()
	{
		avoidCollisions = VARIANT_TRUE;
		barHeight = 50;
		barWidth = 10;
		chartType = chtPieChart;
		lineColor = RGB(128, 128, 128);
		normalizationField = -1;
		radius = 15;
		radius2 = radius * 2;
		rotation = 0.0;
		sizeField = -1;
		thickness = 10.0;
		tilt = 30.0;
		transparency = 255;
		use3Dmode = true;		
		useVariableRadius = false;
		verticalPosition = vpAboveAllLayers;
		visible = true;
		
		USES_CONVERSION;
		valuesFontName = A2BSTR("Arial");
		valuesFrameVisible = VARIANT_TRUE;
		valuesVisible = VARIANT_TRUE;
		valuesFontItalic = VARIANT_FALSE;
		valuesFontBold = VARIANT_FALSE;
		valuesFontColor = RGB(0,0,0);
		valuesFrameColor = RGB(255, 200, 200);
		valuesFontSize = 8;
		valuesStyle = vsHorizontal;
	}

	ChartOptions& operator=(const ChartOptions& options)
	{
		if (this == &options)
			return *this;

		this->avoidCollisions = options.avoidCollisions;
		this->barHeight = options.barHeight;
		this->barWidth = options.barWidth;
		this->chartType = options.chartType;
		this->lineColor = options.lineColor;
		this->normalizationField = options.normalizationField;
		this->radius = options.radius;
		this->radius2 = options.radius2;
		this->rotation = options.rotation;
		this->sizeField = options.sizeField;
		this->thickness = options.thickness;
		this->tilt = options.tilt;
		this->transparency = options.transparency;
		this->use3Dmode = options.use3Dmode;
		this->useVariableRadius = options.useVariableRadius;
		this->verticalPosition = options.verticalPosition;
		this->visible = options.visible;
		
		// values
		this->valuesFontBold = options.valuesFontBold;
		this->valuesFontItalic = options.valuesFontItalic;
		this->valuesFontColor = options.valuesFontColor;
		this->valuesFrameColor = options.valuesFrameColor;
		this->valuesVisible = options.valuesVisible;
		this->valuesFrameVisible = options.valuesFrameVisible;
		this->valuesFontBold = options.valuesFontBold;
		this->valuesFontSize = options.valuesFontSize;
		SysFreeString(this->valuesFontName);
		this->valuesFontName = SysAllocString(options.valuesFontName);
		this->valuesStyle = options.valuesStyle;
	}

	~ChartOptions()
	{
		if (valuesFontName)
			SysFreeString(valuesFontName);
	}
};

// necessary for drawing values
struct ValueRectangle
{
	CString string;
	CRect rect;
};

// -----------------------------------------
//	  CCharts
// -----------------------------------------
class  CCharts
{
public:
	// constructor
	CCharts()
	{
		_key = SysAllocString(L"");
		_caption = SysAllocString(L"");
		_expression = SysAllocString(L"");
		_shapefile = NULL;
		_lastErrorCode = tkNO_ERROR;
		_chartsExist = false;
		_globalCallback = NULL;

		_minVisibleScale = 0.0;
		_maxVisibleScale = 100000000.0;
		_dynamicVisibility = VARIANT_FALSE;

		_collisionBuffer = 0;
		_offsetX = 0;
		_offsetY = 0;

		_savingMode = modeXML;

		gReferenceCounter.AddRef(tkInterface::idCharts);
	}
	
	// destructor
	~CCharts()
	{
		::SysFreeString(_key);
		::SysFreeString(_caption);
		::SysFreeString(_expression);

		Clear();
		ClearFields();

		gReferenceCounter.Release(tkInterface::idCharts);
	}


public:
	void get_Key(/*[out, retval]*/ BSTR *retVal);
	void put_Key(/*[in]*/ BSTR newVal);
	void get_Visible(/*[out, retval]*/ VARIANT_BOOL *retVal);
	void put_Visible(/*[in]*/ VARIANT_BOOL newVal);
	void get_AvoidCollisions(/*[out, retval]*/ VARIANT_BOOL *retVal);
	void put_AvoidCollisions(/*[in]*/ VARIANT_BOOL newVal);
	void get_ChartType(tkChartType* retVal);
	void put_ChartType(tkChartType newVal);
	void get_BarWidth(long* retVal);
	void put_BarWidth(long newVal);
	void get_BarHeight(long* retVal);
	void put_BarHeight(long newVal);
	void get_PieRadius(long* retVal);
	void put_PieRadius(long newVal);
	void get_PieRotation(double* retVal);
	void put_PieRotation(double newVal);
	void get_NumFields(long* retVal);
	void AddField(CChartField* Field, VARIANT_BOOL* retVal);
	void AddField2(long FieldIndex, OLE_COLOR color);
	void InsertField(long Index, CChartField* Field, VARIANT_BOOL* retVal);
	void InsertField2(long Index, long FieldIndex, OLE_COLOR color, VARIANT_BOOL* retVal);
	void RemoveField(long Index, VARIANT_BOOL* vbretval);
	void ClearFields();
	void MoveField(long OldIndex, long NewIndex, VARIANT_BOOL* vbretval);
	void get_Tilt(double* retVal);
	void put_Tilt(double newVal);
	void get_Thickness(double* retVal);
	void put_Thickness(double newVal);
	void get_PieRadius2(LONG* retVal);
	void put_PieRadius2(LONG newVal);
	void get_SizeField(LONG* retVal);
	void put_SizeField(LONG newVal);
	void get_NormalizationField(LONG* retVal);
	void put_NormalizationField(LONG newVal);
	void get_UseVariableRadius(VARIANT_BOOL* retVal);
	void put_UseVariableRadius(VARIANT_BOOL newVal);
	void get_Transparency(SHORT* retVal);
	void put_Transparency(SHORT newVal);
	void get_LineColor(OLE_COLOR* retVal);
	void put_LineColor(OLE_COLOR newVal);
	void get_Use3DMode(VARIANT_BOOL* retVal);
	void put_Use3DMode(VARIANT_BOOL newVal);
	void get_VerticalPosition(tkVerticalPosition* retVal);
	void put_VerticalPosition(tkVerticalPosition newVal);
	void get_Chart(long ShapeIndex, CChart** retVal);
	//void put_Chart(long ShapeIndex, IChart* newVal);
	void get_Field(long FieldIndex, CChartField** retVal);

	void get_GlobalCallback(/*[out, retval]*/ ICallback * *pVal);
	void put_GlobalCallback(/*[in]*/ ICallback * newVal);
	void get_ErrorMsg(/*[in]*/ long ErrorCode, /*[out, retval]*/ BSTR *pVal);
	void get_LastErrorCode(/*[out, retval]*/ long *pVal);
	void get_Count(/*[out, retval]*/ long *pVal);
	void Generate(tkLabelPositioning Position, VARIANT_BOOL* retVal);
	void Clear();
	void DrawChart(int** hdc, float xOrigin, float yOrigin, VARIANT_BOOL hideLabels, OLE_COLOR backColor, VARIANT_BOOL* retVal);
	void get_MaxVisibleScale(double* retval)			{*retval = _maxVisibleScale;		return ;}		
	void put_MaxVisibleScale(double newVal)			{_maxVisibleScale = newVal;		return ;}
	void get_MinVisibleScale(double* retval)			{*retval = _minVisibleScale;		return ;}		
	void put_MinVisibleScale(double newVal)			{_minVisibleScale = newVal;		return ;}
	void get_DynamicVisibility(VARIANT_BOOL* retval)	{*retval = _dynamicVisibility;		return ;}		
	void put_DynamicVisibility(VARIANT_BOOL newVal)	{_dynamicVisibility = newVal;		return ;}

	void get_IconWidth(long* retVal);
	void get_IconHeight(long* retVal);

	void get_Caption(BSTR* retVal);
	void put_Caption(BSTR newVal);
	
	// displaying chart values
	void get_ValuesVisible(VARIANT_BOOL* retval)				{*retval = _options.valuesVisible;		return ;}
	void put_ValuesVisible(VARIANT_BOOL newVal)				{_options.valuesVisible = newVal;		return ;}
	void get_ValuesFrameVisible(VARIANT_BOOL* retval)			{*retval = _options.valuesFrameVisible;	return ;}
	void put_ValuesFrameVisible(VARIANT_BOOL newVal)			{_options.valuesFrameVisible = newVal;	return ;}
	void get_ValuesFontSize(LONG* retval)						{*retval = _options.valuesFontSize;		return ;}
	void put_ValuesFontSize(LONG newVal)						{_options.valuesFontSize = newVal;		return ;}
	void get_ValuesFontItalic(VARIANT_BOOL* retval)			{*retval = _options.valuesFontItalic;	return ;}
	void put_ValuesFontItalic(VARIANT_BOOL newVal)			{_options.valuesFontItalic = newVal;	return ;}		
	void get_ValuesFontBold(VARIANT_BOOL* retval)				{*retval = _options.valuesFontBold;		return ;}
	void put_ValuesFontBold(VARIANT_BOOL newVal)				{_options.valuesFontBold = newVal;		return ;}		
	void get_ValuesFontColor(OLE_COLOR* retval)				{*retval = _options.valuesFontColor;	return ;}
	void put_ValuesFontColor(OLE_COLOR newVal)				{_options.valuesFontColor = newVal;		return ;}
	void get_ValuesFrameColor(OLE_COLOR* retval)				{*retval = _options.valuesFrameColor;	return ;}
	void put_ValuesFrameColor(OLE_COLOR newVal)				{_options.valuesFrameColor = newVal;	return ;}
	void get_ValuesStyle(tkChartValuesStyle* retval)			{*retval = _options.valuesStyle;		return ;}
	void put_ValuesStyle(tkChartValuesStyle newVal)			{_options.valuesStyle = newVal;			return ;}
	void get_ValuesFontName(BSTR* retval);
	void put_ValuesFontName(BSTR newVal);
	void Select(CExtents* BoundingBox, long Tolerance, SelectMode SelectMode, VARIANT* Indices, VARIANT_BOOL* retval);
	void get_VisibilityExpression(BSTR* retval);
	void put_VisibilityExpression(BSTR newVal);

	void get_CollisionBuffer(long* retval)					{*retval = _collisionBuffer;		return ;}		
	void put_CollisionBuffer(long newVal)						{_collisionBuffer = newVal;		return ;}

	void get_OffsetX(LONG* retval)							{*retval = _offsetX;			return ;}
	void put_OffsetX(LONG newVal)								{_offsetX = newVal;			return ;}
	void get_OffsetY(LONG* retval)							{*retval = _offsetY;			return ;}
	void put_OffsetY(LONG newVal)								{_offsetY = newVal;			return ;}

	void Serialize(BSTR* retVal);
	void Deserialize(BSTR newVal);

	void SaveToXML(BSTR Filename, VARIANT_BOOL* retVal);
	void LoadFromXML(BSTR Filename, VARIANT_BOOL* retVal);

	void get_SavingMode(tkSavingMode* retVal);
	void put_SavingMode(tkSavingMode newVal);
	
	void DrawChartVB(int hdc, float x, float y, VARIANT_BOOL hideLabels, OLE_COLOR backColor, VARIANT_BOOL* retVal);

private:
	tkSavingMode _savingMode;
	
	long _lastErrorCode;
	ICallback * _globalCallback;
	BSTR _key;
	BSTR _expression;
	BSTR _caption;

	CShapefile* _shapefile;
	std::vector<CChartField*> _bars;
	ChartOptions _options;
	
	double _maxVisibleScale;
	double _minVisibleScale;
	VARIANT_BOOL _dynamicVisibility;
	
	LONG _collisionBuffer;
	LONG _offsetX;
	LONG _offsetY;

public:
	bool _chartsExist;

public:
	inline void ErrorMessage(long ErrorCode);
	void SetDefaultColorScheme();
	Gdiplus::Color ChangeBrightness(OLE_COLOR color, int shiftValue, long alpha);
	void put_ParentShapefile(CShapefile* newVal);
	CShapefile* get_ParentShapefile();

	ChartOptions* get_UnderlyingOptions() { return &_options; }

	CPLXMLNode* SerializeCore(CString ElementName);
	CPLXMLNode* SerializeChartData(CString ElementName);
	bool DeserializeChartData(CPLXMLNode* node);
	bool DeserializeCore(CPLXMLNode* node);
	VARIANT_BOOL DrawChartCore(CDC* dc, float x, float y, VARIANT_BOOL hideLabels, OLE_COLOR backColor);
};



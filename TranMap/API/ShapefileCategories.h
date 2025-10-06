#pragma once
#include "ShapefileCategory.h"



// CShapefileCategories
class  CShapefileCategories
{
public:
	CShapefileCategories()
	{
		_shapefile = NULL;
		_key = SysAllocString(L"");
		_caption = SysAllocString(L"");
		_globalCallback = NULL;
		_lastErrorCode = tkNO_ERROR;
		_classificationField = -1;
		gReferenceCounter.AddRef(tkInterface::idShapefileCategories);
	}

	~CShapefileCategories()
	{
		::SysFreeString(_key);
		::SysFreeString(_caption);
		_shapefile = NULL;
		for(int i = 0; i< (int)_categories.size(); i++)
			//_categories[i]->Release();
		_categories.clear();

		if (_globalCallback)
		{
			delete _globalCallback;
			_globalCallback = NULL;
		}
		gReferenceCounter.Release(tkInterface::idShapefileCategories);
	}


public:
	void get_Count(long* pVal);
	void get_Key(/*[out, retval]*/ BSTR *pVal);
	void put_Key(/*[in]*/ BSTR newVal);
	void get_GlobalCallback(/*[out, retval]*/ ICallback * *pVal);
	void put_GlobalCallback(/*[in]*/ ICallback * newVal);
	void get_ErrorMsg(/*[in]*/ long ErrorCode, /*[out, retval]*/ BSTR *pVal);
	void get_LastErrorCode(/*[out, retval]*/ long *pVal);
	
	void get_Shapefile(CShapefile** retVal);
	void Add(BSTR Name, CShapefileCategory** retVal);
	void Insert(long Index, BSTR Name, CShapefileCategory** retVal);
	void Remove(long Index, VARIANT_BOOL* vbretval);
	void Clear();
	void get_Item(long Index, CShapefileCategory** retval);
	void put_Item(long Index, CShapefileCategory* newVal);
	void Generate(long FieldIndex, tkClassificationType ClassificationType, long numClasses, VARIANT_BOOL* retVal);
	void ApplyExpressions();
	void ApplyExpression(long CategoryIndex);

	void ApplyColorScheme(tkColorSchemeType Type, CColorScheme* ColorScheme);
	void ApplyColorScheme2(tkColorSchemeType Type, CColorScheme* ColorScheme, tkShapeElements ShapeElement);
	void ApplyColorScheme3(tkColorSchemeType Type, CColorScheme* ColorScheme, tkShapeElements ShapeElement, long CategoryStartIndex, long CategoryEndIndex);

	void get_Caption(/*[out, retval]*/ BSTR *pVal);
	void put_Caption(/*[in]*/ BSTR newVal);

	void MoveUp(long Index, VARIANT_BOOL* retval);
	void MoveDown(long Index, VARIANT_BOOL* retval);
	void Serialize(BSTR* retVal);
	void Deserialize(BSTR newVal);
	void AddRange(long FieldIndex, tkClassificationType ClassificationType, long numClasses, VARIANT minValue, VARIANT maxValue, VARIANT_BOOL* retVal);
	void get_CategoryIndexByName(BSTR categoryName, int* categoryIndex);
	void get_CategoryIndex(CShapefileCategory* category, int* categoryIndex);
	void GeneratePolygonColors(CColorScheme* scheme, VARIANT_BOOL* retval);
	void Sort(LONG FieldIndex, VARIANT_BOOL Ascending, VARIANT_BOOL* retVal);
	void get_ClassificationField(LONG* pVal);
	void put_ClassificationField(LONG newVal);
	void Generate2(BSTR fieldName, tkClassificationType ClassificationType, LONG numClasses, VARIANT_BOOL* retVal);
	
private:
	BSTR _key;
	BSTR _caption;

	long _lastErrorCode;
	ICallback * _globalCallback;
	CShapefile* _shapefile;		// parent shapefile
	long _classificationField;		// used for fast processing of unique values classification; 
									// m_value property of each category (with vt different from VT_EMPTY) will be used in this case
									// should be set to -1 to use the common expression parsing

	std::vector<CShapefileCategory*> _categories;

private:
	void ErrorMessage(long ErrorCode);
	void ApplyExpressionCore(long CategoryIndex);
	bool get_AreaValues(std::vector<double>* values);
	bool get_LengthValues(std::vector<double>* values);
	
public:	
	bool DeserializeCore(CPLXMLNode* node, bool applyExpressions);
	CPLXMLNode* SerializeCore(CString ElementName);

	void put_ParentShapefile(CShapefile* newVal);
	CShapefile* get_ParentShapefile();
	CDrawingOptionsEx* get_UnderlyingOptions(int Index);
	void GenerateCore(std::vector<CategoriesData>* categories, long FieldIndex, tkClassificationType ClassificationType, VARIANT_BOOL* retVal);
	void GetCategoryData(vector<CategoriesData*>& data);
};

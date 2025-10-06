#include "stdafx.h"
#include "ShapefileCategories.h"
#include "ShapefileCategory.h"
#include "ShapeDrawingOptions.h"
#include "Shapefile.h"
#include "Table.h"
#include "TableHelper.h"

//***********************************************************************/
//*			get_Count()
//***********************************************************************/
void  CShapefileCategories::get_Count(long* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*pVal = _categories.size();
	return ;
};

// ***************************************************************
//		Add()
// ***************************************************************
void  CShapefileCategories::Add(BSTR Name, CShapefileCategory** retVal)
{
	this->Insert(_categories.size(), Name, retVal);
	
	return ;
}

// ***************************************************************
//		Insert()
// ***************************************************************
void  CShapefileCategories::Insert(long Index, BSTR Name, CShapefileCategory** retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
		
	if(Index < 0 || Index > (long)_categories.size())
	{	
		ErrorMessage(tkINDEX_OUT_OF_BOUNDS);
		*retVal = VARIANT_FALSE;
		return ;
	}

	*retVal = NULL;
	CShapefileCategory* cat = new CShapefileCategory();
	if (cat == NULL) return ;
	
	// initialization with default options if shapefile is present
	if (_shapefile != NULL)
	{
		CShapeDrawingOptions* defaultOpt = NULL;
		_shapefile->get_DefaultDrawingOptions(&defaultOpt);
		CDrawingOptionsEx* newOpt =((CShapeDrawingOptions*)defaultOpt)->get_UnderlyingOptions();
		delete defaultOpt;
		
		CShapeDrawingOptions* opt = NULL;
		cat->get_DrawingOptions(&opt);
		((CShapeDrawingOptions*)opt)->put_underlyingOptions(newOpt);
		delete opt;
	}
	cat->put_Name(Name);
	
	if (Index == _categories.size())
	{
		_categories.push_back(cat);
	}
	else
	{
		_categories.insert( _categories.begin() + Index, cat);
	}
	
	*retVal = cat;
	((CShapefileCategory*) *retVal)->put_parentCollection(this);

	//cat->AddRef();	// we'll return the category, therefore one more reference
	return ;
}

// ***************************************************************
//		Remove()
// ***************************************************************
void  CShapefileCategories::Remove(long Index, VARIANT_BOOL* vbretval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*vbretval = VARIANT_FALSE;

	if( Index < 0 || Index >= (long)_categories.size() )
	{	
		ErrorMessage(tkINDEX_OUT_OF_BOUNDS);
		*vbretval = VARIANT_FALSE;
	}
	else
	{
		delete _categories[Index];
		_categories[Index] = NULL;
		_categories.erase(_categories.begin() + Index);
		*vbretval = VARIANT_TRUE;
	}
	return ;
}

// ***************************************************************
//		Clear()
// ***************************************************************
void  CShapefileCategories::Clear()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	for (unsigned int i=0; i < _categories.size(); i++ )
	{
		delete _categories[i];
	}
	_categories.clear();

	if (_shapefile)
	{
		std::vector<ShapeData*>* data = ((CShapefile*)_shapefile)->get_ShapeVector();
		for (unsigned  int i = 0; i < data->size(); i++)
		{
			(*data)[i]->category = -1;
		}
	}
	return ;
}

// ***************************************************************
//		get/put_Item()
// ***************************************************************
void  CShapefileCategories::get_Item (long Index, CShapefileCategory** retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if( Index < 0 || Index >= (long)_categories.size() )
	{	
		ErrorMessage(tkINDEX_OUT_OF_BOUNDS);
		*retval = NULL;
	}
	else
	{
		*retval = _categories[Index];
		/*_categories[Index]->AddRef();*/
	}
	return ;
}

void  CShapefileCategories::put_Item(long Index, CShapefileCategory* newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if( Index < 0 || Index >= (long)_categories.size() )
	{	
		ErrorMessage(tkINDEX_OUT_OF_BOUNDS);
		return ;
	}
	else
	{
		if (!newVal)
		{
			ErrorMessage(tkUNEXPECTED_NULL_PARAMETER);
			return ;
		}
		else
		{
			if (_categories[Index] != newVal)
			{
				delete _categories[Index];
				_categories[Index] = newVal;
				//_categories[Index]->AddRef();
			}
			return ;
		}
	}
}

// ***************************************************************
//		AddRange()
// ***************************************************************
// Add categories for a given range
void  CShapefileCategories::AddRange(long FieldIndex, tkClassificationType ClassificationType, long numClasses, VARIANT minValue, VARIANT maxValue, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	USES_CONVERSION;
	*retVal = VARIANT_FALSE;
	
	if(_shapefile == NULL) 
		return ;
	
	CTableClass* tbl = NULL;
	_shapefile->get_Table(&tbl);
	if (!tbl) 
		return ;
		
	std::vector<CategoriesData>* values = tbl->GenerateCategories(FieldIndex, ClassificationType, numClasses, minValue, maxValue);
	
	if (!values)
		return ;
	
	_classificationField = -1;		// fast processing is off

	CShapefileCategory* cat = NULL;
	
	for (int i = 0; i < (int)values->size(); i++ )
	{
		CString strValue;

		CComBSTR bstrName((*values)[i].name);
		CComBSTR bstrExpression((*values)[i].expression);
		this->Add(bstrName, &cat);
		cat->put_Expression(bstrExpression);
		cat->put_MinValue((*values)[i].minValue);
		cat->put_MaxValue((*values)[i].maxValue);
		delete cat;
	}	

	if (ClassificationType == ctUniqueValues)
	{	
		// no fast processing in this mode as user could generate categories by several fields
		// m_classificationField = FieldIndex;		
	}
	
	delete values;
	
	//this->ApplyExpressions();

	*retVal = VARIANT_TRUE;
	return ;
}

// ********************************************************
//     Generate2()
// ********************************************************
void  CShapefileCategories::Generate2(BSTR fieldName, tkClassificationType ClassificationType, LONG numClasses, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = VARIANT_FALSE;

	if (!_shapefile) return ;

	CTableClass* tbl = NULL;
	_shapefile->get_Table(&tbl);
	if (!tbl) return ;

	long fieldIndex;
	tbl->get_FieldIndexByName(fieldName, &fieldIndex);

	Generate(fieldIndex, ClassificationType, numClasses, retVal);

	return ;
}

// ***************************************************************
//		Generate()
// ***************************************************************
void  CShapefileCategories::Generate(long FieldIndex, tkClassificationType ClassificationType, long numClasses, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	USES_CONVERSION;
	*retVal = VARIANT_FALSE;
	
	if(_shapefile == NULL) 
		return ;
	
	CTableClass* tbl = NULL;
	_shapefile->get_Table(&tbl);
	if (!tbl) 
		return ;
		
	std::vector<CategoriesData>* values = tbl->GenerateCategories(FieldIndex, ClassificationType, numClasses);
	if (!values)
		return ;

	GenerateCore(values, FieldIndex, ClassificationType, retVal);

	delete values;

	this->ApplyExpressions();

	*retVal = VARIANT_TRUE;
	return ;
}

// ***************************************************************
//		GenerateCore()
// ***************************************************************
void CShapefileCategories::GenerateCore(std::vector<CategoriesData>* categories, long FieldIndex,
	tkClassificationType ClassificationType, VARIANT_BOOL* retVal)
{
	this->Clear();
	_classificationField = -1;		// fast processing is off

	CShapefileCategory* icat = NULL;
	CShapefileCategory* ct = NULL;

	for (int i = 0; i < (int)categories->size(); i++)
	{
		CString strValue;
		CComBSTR bstrName((*categories)[i].name);
		CComBSTR bstrExpression((*categories)[i].expression);
		
		this->Add(bstrName, &icat);
		icat->put_Expression(bstrExpression);

		icat->put_ValueType(ClassificationType == tkClassificationType::ctUniqueValues ? cvSingleValue : cvRange);
		icat->put_MinValue((*categories)[i].minValue);
		icat->put_MaxValue((*categories)[i].maxValue);
		delete icat;
	}

	// fast processing of categories will be available (m_value will be used without parsing of expressions)
	_classificationField = FieldIndex;
}

//***********************************************************************
//*		get/put_Key()
//***********************************************************************
void  CShapefileCategories::get_Key(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	*pVal = OLE2BSTR(_key);
	return ;
}
void  CShapefileCategories::put_Key(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	::SysFreeString(_key);
	USES_CONVERSION;
	_key = OLE2BSTR(newVal);
	return ;
}

//***********************************************************************
//*		get/put_Caption()
//***********************************************************************
void  CShapefileCategories::get_Caption(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	*pVal = OLE2BSTR(_caption);
	return ;
}
void  CShapefileCategories::put_Caption(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	::SysFreeString(_caption);
	USES_CONVERSION;
	_caption = OLE2BSTR(newVal);
	return ;
}

//***********************************************************************/
//*			ErrorMessage()
//***********************************************************************/
void CShapefileCategories::ErrorMessage(long ErrorCode)
{
	_lastErrorCode = ErrorCode;
	CallbackHelper::ErrorMsg("ShapefileCategories", _globalCallback, _key, ErrorMsg(_lastErrorCode));
}

void  CShapefileCategories::get_LastErrorCode(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*pVal = _lastErrorCode;
	_lastErrorCode = tkNO_ERROR;
	return ;
}

void  CShapefileCategories::get_ErrorMsg(long ErrorCode, BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	*pVal = A2BSTR(ErrorMsg(ErrorCode));
	return ;
}

//***********************************************************************/
//*		get/put_GlobalCallback()
//***********************************************************************/
void  CShapefileCategories::get_GlobalCallback(ICallback **pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*pVal = _globalCallback;
	//if( _globalCallback != NULL ) 
	//	_globalCallback->AddRef();
	return ;
}

void  CShapefileCategories::put_GlobalCallback(ICallback *newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	_globalCallback = newVal;
	return ;
}

// *******************************************************************
//		get_Table()
// *******************************************************************
void  CShapefileCategories::get_Shapefile (CShapefile** retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = _shapefile;

	return ;
}

// *******************************************************************
//		get/put_ParentShapefile()
// *******************************************************************
// For inner use only
void CShapefileCategories::put_ParentShapefile(CShapefile* newVal)
{
	_shapefile = newVal;
}
CShapefile* CShapefileCategories::get_ParentShapefile()
{
	return _shapefile;
}

// *******************************************************************
//		get_UnderlyingCategory()
// *******************************************************************
CDrawingOptionsEx* CShapefileCategories::get_UnderlyingOptions(int Index)
{
	if (Index >=0 && Index < (int)_categories.size())
		return ((CShapefileCategory*)_categories[Index])->get_UnderlyingOptions();
	else
		return NULL;
}

// *******************************************************************
//		ApplyExpressions()
// *******************************************************************
void  CShapefileCategories::ApplyExpressions()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	ApplyExpressionCore(-1);
	return ;
}

// *******************************************************************
//		ApplyExpression()
// *******************************************************************
void  CShapefileCategories::ApplyExpression(long CategoryIndex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	// switching off shapes that are currently included in the category
	std::vector<ShapeData*>* data = ((CShapefile*)_shapefile)->get_ShapeVector();
	for (unsigned int i = 0; i < data->size(); i++)
	{
		if ((*data)[i]->category == CategoryIndex)
		{
			(*data)[i]->category = -1;
		}
	}

	ApplyExpressionCore(CategoryIndex);
	return ;
}

// *******************************************************************
//		ApplyExpressionCore
// *******************************************************************
void CShapefileCategories::ApplyExpressionCore(long CategoryIndex)
{
	if (!_shapefile)
		return;
	
	CTableClass* tbl = NULL;
	_shapefile->get_Table(&tbl);
	if ( !tbl )	return;
	
	long numShapes;
	_shapefile->get_NumShapes(&numShapes);
		
	// vector of numShapes size with category index for each shape
	std::vector<int> results;
	results.resize(numShapes, -1);

	bool uniqueValues = true;
	for (unsigned int i = 0; i < _categories.size(); i++) {
		tkCategoryValue value;
		_categories[i]->get_ValueType(&value);
		if (value != cvSingleValue) {
			uniqueValues = false;
			break;
		}
	}

	// ----------------------------------------------------------------
	// we got unique values classification and want to process it fast
	// ----------------------------------------------------------------
	bool parsingIsNeeded = true;	
	if (_classificationField != -1 && uniqueValues)
	{
		parsingIsNeeded = false;	// in case there are unique values only we don't need any parsing
			
		std::map<CComVariant, long> myMap;				// variant value as key and number of category as result
		for (unsigned int i = 0; i < _categories.size(); i++)
		{
			if (i == CategoryIndex || CategoryIndex == -1 )
			{
				CComVariant val;
				_categories[i]->get_MinValue(&val);
				if (val.vt != VT_EMPTY)
				{
					CComVariant val2;
					VariantCopy(&val2, &val);
					myMap[val2] = i;
				}
			}
		}
		
		// applying categories to shapes
		VARIANT val;
		VariantInit(&val);
		for (long i = 0; i < numShapes; i++)
		{
			tbl->get_CellValue(_classificationField, i, &val);
			if (myMap.find(val) != myMap.end())
			{
				results[i] = myMap[val];	// writing the index of category
			}
		}
		VariantClear(&val);
	}
		
	// -------------------------------------------------------------
	//		Analyzing expressions
	// -------------------------------------------------------------
	if (parsingIsNeeded)
	{
		// building list of expressions
		std::vector<CString> expressions;
		for (unsigned int i = 0; i < _categories.size(); i++)
		{
			if (i == CategoryIndex || CategoryIndex == -1 )
			{
				CComVariant val;
				_categories[i]->get_MinValue(&val);
				
				CComBSTR expr;
				_categories[i]->get_Expression(&expr);
				USES_CONVERSION;
				CString str = OLE2CA(expr);
				expressions.push_back(str);
			}
			else
			{
				// we don't need this categories, so dummy strings for them
				CString str = "";
				expressions.push_back(str);
			}
		}

		// adding category indices for shapes in the results vector
		tbl->AnalyzeExpressions(expressions, results);
	}
		
	// saving results
	if (CategoryIndex == -1 )
	{
		for (unsigned long i = 0; i < results.size(); i++)
		{
			_shapefile->put_ShapeCategory(i, results[i]);
		}
	}
	else
	{
		for (unsigned long i = 0; i < results.size(); i++)
		{
			if (results[i] == CategoryIndex)
				_shapefile->put_ShapeCategory(i, CategoryIndex);
		}
	}
}

// ********************************************************
//	  ApplyColorScheme()
// ********************************************************
void  CShapefileCategories::ApplyColorScheme (tkColorSchemeType Type, CColorScheme* ColorScheme)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	ApplyColorScheme2(Type, ColorScheme, shElementDefault);
	return ;
}

// ********************************************************
//	  ApplyColorScheme2()
// ********************************************************
void  CShapefileCategories::ApplyColorScheme2 (tkColorSchemeType Type, CColorScheme* ColorScheme, tkShapeElements ShapeElement)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	ApplyColorScheme3(Type, ColorScheme, ShapeElement, 0, _categories.size() - 1);
	return ;
}

// ********************************************************
//	  ApplyColorScheme3()
// ********************************************************
void  CShapefileCategories::ApplyColorScheme3 (tkColorSchemeType Type, CColorScheme* ColorScheme, 
													  tkShapeElements ShapeElement, long CategoryStartIndex, long CategoryEndIndex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	if (!_shapefile)
	{
		ErrorMessage(tkPARENT_SHAPEFILE_NOT_EXISTS);
		return ;
	}

	long numBreaks;
	ColorScheme->get_NumBreaks(&numBreaks);

	if (numBreaks <= 1)
	{
		ErrorMessage(tkCOLOR_SCHEME_IS_EMPTY);
		return ;
	}

	// we'll correct invalid indices
	if (CategoryEndIndex >= (long)_categories.size())
	{
		CategoryEndIndex = (long(_categories.size() - 1));
	}

	if (CategoryStartIndex < 0)
	{
		CategoryStartIndex = 0;
	}

	if ( CategoryEndIndex == CategoryStartIndex )
	{
		CallbackHelper::ErrorMsg("Can't apply color scheme to a single a single category (start index and end index are the same.)");
		ErrorMessage(tkINVALID_PARAMETER_VALUE);
		return ;
	}

	double maxValue;
	ColorScheme->get_BreakValue(numBreaks - 1, &maxValue);
	
	// choosing the element to apply colors to
	if ( ShapeElement == shElementDefault)
	{
		ShpfileType shpType;
		_shapefile->get_ShapefileType(&shpType);
		
		if (shpType == SHP_POINT || shpType == SHP_POINTM || shpType == SHP_POINTZ ||
			shpType == SHP_MULTIPOINT || shpType == SHP_MULTIPOINTM || shpType == SHP_MULTIPOINTZ ||
			shpType == SHP_POLYGON || shpType == SHP_POLYGONM || shpType == SHP_POLYGONZ)
		{
			ShapeElement = shElementFill;
		}
		else if (shpType == SHP_POLYLINE || shpType == SHP_POLYLINEM || shpType == SHP_POLYLINEZ)
		{
			ShapeElement = shElementLines;
		}
	}

	CShapeDrawingOptions* options = NULL;
	for (int i = CategoryStartIndex; i <= CategoryEndIndex; i++)
	{
		OLE_COLOR color;
		double value;
		if ( Type == ctSchemeRandom )			
		{
			value = double(rand()/double(RAND_MAX));
			ColorScheme->get_RandomColor(value, &color);
		}
		else if ( Type == ctSchemeGraduated )	
		{
			value = double(i - CategoryStartIndex)/double(CategoryEndIndex - CategoryStartIndex) * maxValue;
			ColorScheme->get_GraduatedColor(value, &color);
		}
		
		_categories[i]->get_DrawingOptions(&options);
		
		if ( options )
		{
			switch (ShapeElement)
			{
				case shElementFill:				options->put_FillColor(color);		break;
				case shElementFill2:			options->put_FillColor2(color);		break;
				case shElementLines:			options->put_LineColor(color);		break;
				case shElementFillBackground:	options->put_FillBgColor(color);	break;
			}
			delete options;
		}
	}
	return ;
}

// ***************************************************************
//		MoveUp
// ***************************************************************
void  CShapefileCategories::MoveUp (long Index, VARIANT_BOOL* retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if (Index < (long)_categories.size() && Index > 0)
	{
		CShapefileCategory* catBefore = _categories[Index - 1];
		_categories[Index - 1] = _categories[Index];
		_categories[Index] = catBefore;
		
		std::vector<ShapeData*>* data = ((CShapefile*)_shapefile)->get_ShapeVector();
		for (unsigned int i = 0; i < data->size(); i++)
		{
			if ((*data)[i]->category == Index)
			{
				(*data)[i]->category = Index - 1;
			}
			else if ((*data)[i]->category == Index - 1)
			{
				(*data)[i]->category = Index;
			}
		}

		*retval = VARIANT_TRUE;
	}
	else
	{
		ErrorMessage(tkINDEX_OUT_OF_BOUNDS);
		*retval = VARIANT_FALSE;
	}
	return ;
}

// ***************************************************************
//		MoveDown
// ***************************************************************
void  CShapefileCategories::MoveDown (long Index, VARIANT_BOOL* retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if (Index < (long)_categories.size() - 1 && Index >= 0)
	{
		CShapefileCategory* catAfter = _categories[Index + 1];
		_categories[Index + 1] = _categories[Index];
		_categories[Index] = catAfter;

		std::vector<ShapeData*>* data = ((CShapefile*)_shapefile)->get_ShapeVector();
		for (unsigned int i = 0; i < data->size(); i++)
		{
			if ((*data)[i]->category == Index)
			{
				(*data)[i]->category = Index + 1;
			}
			else if ((*data)[i]->category == Index + 1)
			{
				(*data)[i]->category = Index;
			}
		}
		*retval = VARIANT_TRUE;
	}
	else
	{
		ErrorMessage(tkINDEX_OUT_OF_BOUNDS);
		*retval = VARIANT_FALSE;
	}
	return ;
}


// ********************************************************
//     Serialize()
// ********************************************************
void  CShapefileCategories::Serialize(BSTR* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	CPLXMLNode* psTree = this->SerializeCore("ShapefileCategoriesClass");
	Utility::SerializeAndDestroyXmlTree(psTree, retVal);
	return ;
}

// ********************************************************
//     SerializeCore()
// ********************************************************
CPLXMLNode* CShapefileCategories::SerializeCore(CString ElementName)
{
	USES_CONVERSION;
	
	CPLXMLNode* psTree = CPLCreateXMLNode( NULL, CXT_Element, ElementName);
	CString str;

	// classification field
	Utility::CPLCreateXMLAttributeAndValue(psTree, "ClassificationField", CPLString().Printf("%d", _classificationField));

	// field type
	FieldType type;
	CTableClass* table = NULL;
	_shapefile->get_Table(&table);
	if (table && _classificationField != -1)
	{
		CField* fld = NULL;
		table->get_Field(_classificationField, &fld);
		if (fld)
		{
			fld->get_Type(&type);
			Utility::CPLCreateXMLAttributeAndValue( psTree, "FieldType", CPLString().Printf("%d", (int)type));
		}
	}
	
	for (unsigned int i = 0; i < _categories.size(); i++)
	{
		CPLXMLNode* psNode = CPLCreateXMLNode(psTree, CXT_Element, "ShapefileCategoryClass");
		
		// name
		BSTR vbstr;
		_categories[i]->get_Name(&vbstr);
		Utility::CPLCreateXMLAttributeAndValue( psNode, "Name", OLE2CA(vbstr));
		SysFreeString(vbstr);

		// expression
		_categories[i]->get_Expression(&vbstr);
		str = OLE2CA(vbstr);

		Utility::CPLCreateXMLAttributeAndValue( psNode, "Expression", str);
		SysFreeString(vbstr);

		CShapefileCategory* ct = (CShapefileCategory*)_categories[i];
		Utility::CPLCreateXMLAttributeAndValue(psNode, "ValueType", CPLString().Printf("%d", (int)ct->GetCategoryValue()));

		// values
		Utility::SerializeVariant(psNode, "Value", ct->GetMinValue());
		Utility::SerializeVariant(psNode, "MaxValue", ct->GetMaxValue());
		
		// options
		CShapeDrawingOptions* options;
		_categories[i]->get_DrawingOptions(&options);
		CPLXMLNode* psChild = ((CShapeDrawingOptions*)options)->SerializeCore("ShapeDrawingOptionsClass");
		if (psChild)
		{
			CPLAddXMLChild(psNode, psChild);
		}
		delete options;
	}
	return psTree;
}

// ********************************************************
//     DeserializeCore()
// ********************************************************
bool CShapefileCategories::DeserializeCore(CPLXMLNode* node, bool applyExpressions)
{
	if (!node)
		return false;
	
	// removing existing categories
	this->Clear();

	_classificationField = -1;
	bool loadField = false;

	FieldType type = INTEGER_FIELD;
	CString s = CPLGetXMLValue( node, "FieldType", NULL );
	if (s != "")
	{
		type = (FieldType)atoi(s);
		loadField = true;
	}

	if (loadField)
	{
		s = CPLGetXMLValue( node, "ClassificationField", NULL );
		if (s != "")
		{
			int fieldIndex = atoi(s);

			CTableClass* table = NULL;
			_shapefile->get_Table(&table);
			if (table)
			{
				CField* fld = NULL;
				table->get_Field(fieldIndex, &fld);
				if (fld)
				{
					FieldType realType;
					fld->get_Type(&realType);

					if (type == realType)		// a check by name can be added as well
					{
						_classificationField = fieldIndex;
					}
					delete fld;
				}
			}
		}
	}
	

	node = node->psChild;
	if (node)
	{
		while (node)
		{
			if (strcmp(node->pszValue, "ShapefileCategoryClass") == 0)
			{
				CString str;
				str = CPLGetXMLValue( node, "Name", NULL );
				CComBSTR bstrName( str );
				CShapefileCategory* cat = NULL;
				this->Add(bstrName, &cat);

				str = CPLGetXMLValue( node, "Expression", NULL );
				CComBSTR bstrExpression(str);
				cat->put_Expression(bstrExpression);

				str = CPLGetXMLValue(node, "ValueType", NULL);
				tkCategoryValue ctVal = (str != "") ? (tkCategoryValue)atoi(str.GetString()) : cvExpression;
				cat->put_ValueType(ctVal);

				str = CPLGetXMLValue(node, "Value", NULL);
				Utility::DeserializeVariant(str, type, ((CShapefileCategory*)cat)->GetMinValue());

				str = CPLGetXMLValue(node, "MaxValue", NULL);
				Utility::DeserializeVariant(str, type, ((CShapefileCategory*)cat)->GetMaxValue());

				// drawing options
				CPLXMLNode* nodeOptions = CPLGetXMLNode( node, "ShapeDrawingOptionsClass" );
				if (nodeOptions)
				{
					CShapeDrawingOptions* options = NULL;
					cat->get_DrawingOptions(&options);
					((CShapeDrawingOptions*)options)->DeserializeCore(nodeOptions);
					delete options;
				}
				delete cat;
			}
			node = node->psNext;
		} 

		if (applyExpressions)
			this->ApplyExpressions();
	}
	return true;
}

// ********************************************************
//     Deserialize()
// ********************************************************
void  CShapefileCategories::Deserialize(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;

	CString str = OLE2CA(newVal);
	CPLXMLNode* node = CPLParseXMLString(str.GetString());
	if (node)
	{
		CPLXMLNode* nodeCat = CPLGetXMLNode(node, "=ShapefileCategoriesClass");
		if (nodeCat)
		{
			DeserializeCore(nodeCat, true);
		}
		CPLDestroyXMLNode(node);
	}
	return ;
}

// ********************************************************
//     get_CategoryIndexByName()
// ********************************************************
void  CShapefileCategories::get_CategoryIndexByName(BSTR categoryName, int* categoryIndex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*categoryIndex = -1;
	for(size_t i = 0; i < _categories.size(); i++)
	{
		CComBSTR bstr;
		_categories[i]->get_Name(&bstr);
		
		if (lstrcmpW(bstr, categoryName) == 0)
		{
			*categoryIndex = i;
			break;
		}
	}
	return ;
}

// ********************************************************
//     get_CategoryIndex()
// ********************************************************
void  CShapefileCategories::get_CategoryIndex(CShapefileCategory* category, int* categoryIndex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*categoryIndex = -1;
	for(size_t i = 0; i < _categories.size(); i++)
	{
		if (_categories[i] == category)
		{
			*categoryIndex = i;
			break;
		}
	}
	return ;
}

// ********************************************************
//     GenerateColorForPolygons()
// ********************************************************
void  CShapefileCategories::GeneratePolygonColors(CColorScheme* scheme, VARIANT_BOOL* retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retval = VARIANT_FALSE;
	
	// -------------------------------------------------
	// parameter validation
	// -------------------------------------------------
	if(!_shapefile) {
		ErrorMessage(tkPARENT_SHAPEFILE_NOT_EXISTS);
		return ;
	}

	ShpfileType shpType;
	_shapefile->get_ShapefileType(&shpType);
	if (Utility::ShapeTypeConvert2D(shpType) != SHP_POLYGON)
	{
		ErrorMessage(tkUNEXPECTED_SHAPE_TYPE);
		return ;
	}

	bool tempScheme = false;
	if (!scheme)
	{
		// let's create a scheme if none is provided
		scheme = new CColorScheme();
		scheme->AddBreak(0.0, RGB(255, 179, 71));
		scheme->AddBreak(0.1, RGB(253, 253, 150));
		scheme->AddBreak(0.2, RGB(119, 221, 119));
		scheme->AddBreak(0.3, RGB(174, 198, 207));
		scheme->AddBreak(0.4, RGB(222, 165, 164));
		scheme->AddBreak(0.5, RGB(255, 105, 97));
		scheme->AddBreak(0.6, RGB(130, 105, 83));
		scheme->AddBreak(0.7, RGB(100, 20, 100));
		scheme->AddBreak(1.0, RGB(207, 207, 196));
		//scheme->SetColors4(PredefinedColorScheme::SummerMountains);
		tempScheme= true;
	}
	
	// -------------------------------------------------
	//  do the processing
	// -------------------------------------------------
	Coloring::ColorGraph* graph = ((CShapefile*)_shapefile)->GeneratePolygonColors();
	if (graph)
	{
		int colorCount = graph->GetColorCount();

		// -------------------------------------------------
		// create categories
		// -------------------------------------------------
		int firstCategory = _categories.size();
		long numBreaks;
		scheme->get_NumBreaks(&numBreaks);
		for(int i = 0; i < colorCount; i++)
		{
			CString s;
			s.Format("Color %d", i + 1);
			CShapefileCategory* ct = NULL;

			CComBSTR bstrName(s);
			this->Add(bstrName, &ct);
			CDrawingOptionsEx* opt = ((CShapefileCategory*)ct)->get_UnderlyingOptions();
			
			OLE_COLOR clr;
			if (colorCount <= numBreaks)
			{
				// we can use colors without interpolation
				scheme->get_BreakColor(i, &clr);
			}
			else
			{
				// must use interpolation
				scheme->get_GraduatedColor(i/(double)colorCount, &clr);
			}

			opt->fillColor = clr;
			delete ct;
		}

		// -------------------------------------------------
		// apply indices to polygons
		// -------------------------------------------------
		for(size_t i = 0; i < graph->nodes.size(); i++)
		{
			int shapeId = graph->nodes[i]->id;
			_shapefile->put_ShapeCategory(shapeId, firstCategory + graph->nodes[i]->color);
		}
		delete graph;
	}

	if (tempScheme)
		delete scheme;
	return ;
}

// ********************************************************
//     Sort()
// ********************************************************
// TODO: implement
void  CShapefileCategories::Sort(LONG FieldIndex, VARIANT_BOOL Ascending, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = VARIANT_FALSE;

	if (!_shapefile)
		return ;

	LONG numFields;
	_shapefile->get_NumFields(&numFields);
	if (FieldIndex < 0 || FieldIndex >= numFields)
	{
		ErrorMessage(tkINDEX_OUT_OF_BOUNDS);
		return ;
	}
	
	if (_categories.size() < 2)
	{
		// nothing to sort
		return ;
	}

	CTableClass* table = NULL;
	_shapefile->get_Table(&table);
	if (!table) return ;

	CField* fld = NULL;
	table->get_Field(FieldIndex, &fld);
	FieldType type;
	fld->get_Type(&type);
	delete fld;
	CComVariant valDefault;
	if (type == DOUBLE_FIELD || INTEGER_FIELD)
	{
		valDefault = 0;
	}
	else
	{
		valDefault = "";
	}
		
	multimap <CComVariant, CShapefileCategory*> map;

	for (unsigned int i = 0; i < _categories.size(); i++)
	{
		VARIANT_BOOL vbretval = VARIANT_FALSE;
		CComVariant var = NULL;
		BSTR expr;
		_categories[i]->get_Expression(&expr);

		// TODO: implement
		//table->CalculateStat(FieldIndex, Operation, expr, &var, &vbretval);

		if (vbretval)
		{
			pair<CComVariant, CShapefileCategory*> myPair(var, _categories[i]);	
			map.insert(myPair);	
		}
		else
		{
			pair<CComVariant, CShapefileCategory*> myPair(valDefault, _categories[i]);	
			map.insert(myPair);	
		}
	}
	
	// returning result
	multimap <CComVariant, CShapefileCategory*>::iterator p;
	p = map.begin();
		
	int i = 0;
	ASSERT(map.size() == _categories.size());
	
	while(p != map.end())
	{
		CShapefileCategory* cat = p->second;
		_categories[i] = p->second;
		i++; p++;
	}

	*retVal = VARIANT_TRUE;
	return ;
}

// ********************************************************
//     ClassificationField()
// ********************************************************
void  CShapefileCategories::get_ClassificationField(LONG* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _classificationField;
	return ;
}
void  CShapefileCategories::put_ClassificationField(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_classificationField = newVal;
	return ;
}

// ********************************************************
//     GetCategoryData()
// ********************************************************
void CShapefileCategories::GetCategoryData(vector<CategoriesData*>& data)
{
	USES_CONVERSION;
	data.clear();
	for (size_t i = 0; i < _categories.size(); i++) 
	{
		CategoriesData* ct = new CategoriesData();
		_categories[i]->get_MinValue(&ct->minValue);
		_categories[i]->get_MaxValue(&ct->maxValue);
		_categories[i]->get_ValueType(&ct->valueType);
		CComBSTR expr;
		_categories[i]->get_Expression(&expr);
		ct->expression = OLE2A(expr);
		ct->classificationField = _classificationField;
		data.push_back(ct);
	}
}


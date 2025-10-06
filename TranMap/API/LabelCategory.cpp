#include "stdafx.h"
#include "LabelCategory.h"
#include "cpl_minixml.h"
#include "cpl_string.h"

// *****************************************************************
//		get/put_Name
// *****************************************************************
void  CLabelCategory::get_Name(BSTR* retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	*retval = OLE2BSTR(_name);
	return ;
}
void  CLabelCategory::put_Name(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	::SysFreeString(_name);
	_name = OLE2BSTR(newVal);
	return ;
}

// *****************************************************************
//		get/put_Expression
// *****************************************************************
void  CLabelCategory::get_Expression(BSTR* retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	*retval = OLE2BSTR(_expression);
	return ;
}
void  CLabelCategory::put_Expression(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	::SysFreeString(_expression);
	_expression = OLE2BSTR(newVal);
	return ;
}

// *****************************************************************
//		get/put_StartValue
// *****************************************************************
void  CLabelCategory::get_MinValue(VARIANT *pVal)
{
	VariantClear(pVal);
	VariantInit(pVal);
	VariantCopy(pVal,&_minValue);
	return ;
}

void  CLabelCategory::put_MinValue(VARIANT newVal)
{
	VariantClear(&_minValue);	
	VariantInit(&_minValue);	
	VariantCopy(&_minValue,&newVal);
	return ;
}

// *****************************************************************
//		get/put_EndValue
// *****************************************************************
void  CLabelCategory::get_MaxValue(VARIANT *pVal)
{
	VariantClear(pVal);
	VariantInit(pVal);
	VariantCopy(pVal,&_maxValue);
	return ;
}
void  CLabelCategory::put_MaxValue(VARIANT newVal)
{
	VariantClear(&_maxValue);
	VariantInit(&_maxValue);	
	VariantCopy(&_maxValue,&newVal);
	return ;
}


// ////////////////////////////////////////////////////////////// //
//	PROPERTIES COMMON FOR CLABELS AND CLABELCATEGORY
//	CAN BE COPIED TO THE OTHER CLASS; DON'T BREAK THIS BLOCK
// ////////////////////////////////////////////////////////////// //

// *****************************************************************
//		FontName()
// *****************************************************************
void  CLabelCategory::get_FontName(BSTR* retval)					
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	*retval = OLE2BSTR(_options.fontName);		
	return ;
};	
void  CLabelCategory::put_FontName(BSTR newVal)					
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	::SysFreeString(_options.fontName);
	_options.fontName = OLE2BSTR(newVal);
	return ;
};	

// *****************************************************************
//		Font/FrameTransparency()
// *****************************************************************
void  CLabelCategory::get_FontTransparency(long* retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retval = _options.fontTransparency;		
	return ;
};		
void  CLabelCategory::put_FontTransparency(long newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if (newVal < 0) newVal = 0;
	if (newVal > 255) newVal = 255;
	_options.fontTransparency = newVal;
	return ;
};
void  CLabelCategory::get_FrameTransparency(long* retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retval = _options.frameTransparency;		
	return ;
};		
void  CLabelCategory::put_FrameTransparency(long newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if (newVal < 0) newVal = 0;
	if (newVal > 255) newVal = 255;
	_options.frameTransparency = newVal;
	return ;
};

// *****************************************************************
//			Font style options
// *****************************************************************
void  CLabelCategory::get_FontItalic(VARIANT_BOOL* retval)
{
	*retval = ((_options.fontStyle & fstItalic)?VARIANT_TRUE:VARIANT_FALSE);
	return ;
};
void  CLabelCategory::put_FontItalic(VARIANT_BOOL newVal)
{
	if (newVal)		_options.fontStyle |= fstItalic;
	else			_options.fontStyle &= (0xFFFFFFFF ^ fstItalic);
	return ;
};
void  CLabelCategory::get_FontBold(VARIANT_BOOL* retval)
{
	*retval = ((_options.fontStyle & fstBold)?VARIANT_TRUE:VARIANT_FALSE);
	return ;
};
void  CLabelCategory::put_FontBold(VARIANT_BOOL newVal)
{
	if (newVal)		_options.fontStyle |= fstBold;
	else			_options.fontStyle &= (0xFFFFFFFF ^ fstBold);
	return ;
};
void  CLabelCategory::get_FontUnderline(VARIANT_BOOL* retval)
{
	*retval = ((_options.fontStyle & fstUnderline)?VARIANT_TRUE:VARIANT_FALSE);
	return ;
};
void  CLabelCategory::put_FontUnderline(VARIANT_BOOL newVal)
{
	if (newVal)		_options.fontStyle |= fstUnderline;
	else			_options.fontStyle &= (0xFFFFFFFF ^ fstUnderline);
	return ;
};
void  CLabelCategory::get_FontStrikeOut(VARIANT_BOOL* retval)
{
	*retval = ((_options.fontStyle & fstStrikeout)?VARIANT_TRUE:VARIANT_FALSE);
	return ;
};
void  CLabelCategory::put_FontStrikeOut(VARIANT_BOOL newVal)
{
	if (newVal)		_options.fontStyle |= fstStrikeout;
	else			_options.fontStyle &= (0xFFFFFFFF ^ fstStrikeout);
	return ;
};
////////////////////////////////////////////////////////////////
//      END OF COMMON OPTIONS
////////////////////////////////////////////////////////////////

// *************************************************************
//		get_LabelOptions
// *************************************************************
CLabelOptions* CLabelCategory::get_LabelOptions()
{
	return &_options;
}
void CLabelCategory::put_LabelOptions(CLabelOptions* newVal)
{
	_options =	*newVal;	// redefined operator
}

// ********************************************************
//     Serialize()
// ********************************************************
void  CLabelCategory::Serialize(BSTR* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	CPLXMLNode* node = SerializeCore("LabelCategoryClass");
	Utility::SerializeAndDestroyXmlTree(node, retVal);
	return ;
}

// ********************************************************
//     Serialize()
// ********************************************************
CPLXMLNode* CLabelCategory::SerializeCore(CString ElementName)
{
	CPLXMLNode* node = _options.SerializeToTree(ElementName);
	if (node)
	{
		// non-drawing properties
		USES_CONVERSION;
		CString str = OLE2CA(_name);
		Utility::CPLCreateXMLAttributeAndValue( node, "Name", str);
		
		str = OLE2CA(_expression);
		Utility::CPLCreateXMLAttributeAndValue( node, "Expression", str);
		Utility::CPLCreateXMLAttributeAndValue( node, "Enabled", CPLString().Printf("%d", (int)_enabled ));
		
		// TODO: do we really need to serialize these ?
		//VARIANT m_minValue;
		//VARIANT m_maxValue;
	}
	return node;
}

// ********************************************************
//     Deserialize()
// ********************************************************
void  CLabelCategory::Deserialize(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	CString s = OLE2CA(newVal);
	CPLXMLNode* node = CPLParseXMLString(s.GetString());
	this->DeserializeFromNode(node);
	CPLDestroyXMLNode(node);
	return ;
}

// ********************************************************
//     DeserializeFromNode()
// ********************************************************
void CLabelCategory::DeserializeFromNode(CPLXMLNode* node)
{
	if (!node)
		return;
	
	_options.DeserializeFromNode(node);

	// non-drawing properties
	CString s = CPLGetXMLValue( node, "Name", NULL );
	if (s != "")
	{
		CComBSTR bstr(s);
		put_Name(bstr);
	}

	s = CPLGetXMLValue( node, "Expression", NULL );
	if (s != "")
	{
		s.Replace("&#xA;", "\r\n");
		SysFreeString(_expression);
		_expression = A2BSTR(s);
	}

	s = CPLGetXMLValue( node, "Enabled", NULL );
	if (s != "") _enabled = (VARIANT_BOOL)atoi(s.GetString());
}


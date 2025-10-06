#include "stdafx.h"
#include "Field.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// *********************************************************************
//		get/put_Name
// *********************************************************************
void  CField::get_Name(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	*pVal = OLE2BSTR(_name);
	return ;
}

void  CField::put_Name(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	
	if( _tcslen( OLE2CA(newVal) ) > 0 )
	{	
		if (!CheckTableEditingState()) return ;
		::SysFreeString(_name);
		_name = OLE2BSTR(newVal);
		_isUpdated = true;
	}
	else
		ErrorMessage(tkZERO_LENGTH_STRING);
	
	return ;
}

// *********************************************************************
//		get/put_Width
// *********************************************************************
void  CField::get_Width(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*pVal = _width;
	return ;
}
void  CField::put_Width(long newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if( newVal >= 0 )
	{
		if (!CheckTableEditingState()) return ;
		_width = newVal;
		_isUpdated = true;
	}
	else
		ErrorMessage(tkINVALID_PARAMETER_VALUE);

	return ;
}

// *********************************************************************
//		get/put_Precision
// *********************************************************************
void  CField::get_Precision(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*pVal = _precision;
	return ;
}

void  CField::put_Precision(long newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if( newVal >= 0 )
	{
		if (!CheckTableEditingState()) return ;
		_precision = newVal;
		_isUpdated = true;
	}
	else
		ErrorMessage(tkINVALID_PARAMETER_VALUE);
	
	return ;
}

// *********************************************************************
//		get/put_Type
// *********************************************************************
void  CField::get_Type(FieldType *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*pVal = _type;
	return ;
}

void  CField::put_Type(FieldType newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if (!CheckTableEditingState()) return ;
	_type = newVal;
	_isUpdated = true;
	return ;
}

// *********************************************************************
//		CheckTableEditingState
// *********************************************************************
bool CField::CheckTableEditingState()
{
	if (_table != NULL)
	{
		VARIANT_BOOL editing;
		_table->get_EditingTable(&editing);
		if(!editing)
		{
			ErrorMessage(tkDBF_NOT_IN_EDIT_MODE);
			return false;
		}
		else
		{
			return true;
		}
	}
	else
		return true;	// if there is no parent table, we can change the settings freely
	
}

// *********************************************************************
//		get_LastErrorCode
// *********************************************************************
void  CField::get_LastErrorCode(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*pVal = _lastErrorCode;
	_lastErrorCode = tkNO_ERROR;
	return ;
}

// *********************************************************************
//		get_ErrorMsg
// *********************************************************************
void  CField::get_ErrorMsg(long ErrorCode, BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	*pVal = A2BSTR(ErrorMsg(ErrorCode));
	return ;
}

// *********************************************************************
//		get/put_GlobalCallback
// *********************************************************************
void  CField::get_GlobalCallback(ICallback **pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = _globalCallback;
	return ;
}

void  CField::put_GlobalCallback(ICallback *newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	_globalCallback = newVal;
	return ;
}

// *********************************************************************
//		get/put_Key
// *********************************************************************
void  CField::get_Key(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	USES_CONVERSION;
	*pVal = OLE2BSTR(_key);

	return ;
}

void  CField::put_Key(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	USES_CONVERSION;
	::SysFreeString(_key);
	_key = OLE2BSTR(newVal);

	return ;
}

// **************************************************************
//		ErrorMessage()
// **************************************************************
void CField::ErrorMessage(long ErrorCode)
{
	_lastErrorCode = ErrorCode;
	CallbackHelper::ErrorMsg("Field", _globalCallback, _key, ErrorMsg(_lastErrorCode));
}

// **************************************************************
//		Clone()
// **************************************************************
void  CField::Clone(/*[out, retval]*/ CField** retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	CField* fld = new CField();
	//CoCreateInstance(CLSID_Field,NULL,CLSCTX_INPROC_SERVER,IID_CField,(void**)&fld);
	fld->put_Precision(_precision);
	fld->put_Type(_type);
	fld->put_Name(_name);
	fld->put_Width(_width);
	*retVal = fld;
	return ;
}
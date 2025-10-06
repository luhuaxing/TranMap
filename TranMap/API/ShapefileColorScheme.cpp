
#include "stdafx.h"
#include "ShapefileColorScheme.h"

#ifdef OLD_API

/////////////////////////////////////////////////////////////////////////////
// CShapefileColorScheme

void  CShapefileColorScheme::NumBreaks(long *result)
{
	
	*result = _allBreaks.size();	
	return ;
}

void  CShapefileColorScheme::Remove(long Index)
{
	
	if( Index >= 0 && Index < (int)_allBreaks.size() )
	{	if( _allBreaks[Index] != NULL )
			_allBreaks[Index]->Release();
		_allBreaks.erase( _allBreaks.begin() + Index );
	}
	else
	{	
		ErrorMessage(tkINDEX_OUT_OF_BOUNDS);
	}
	return ;
}

void  CShapefileColorScheme::Add(CShapefileColorBreak *Break, long *result)
{
	if( Break == NULL )
		return ;

	
	*result = _allBreaks.size();
	Break->AddRef();
	_allBreaks.push_back( Break );
	return ;
}

void  CShapefileColorScheme::InsertAt(int Position, CShapefileColorBreak *Break, long *result)
{
	if( Break == NULL )
		return ;

	
	*result = _allBreaks.size();
	Break->AddRef();
	_allBreaks.insert(_allBreaks.begin() + Position, Break);
	return ;
}

void  CShapefileColorScheme::get_ColorBreak(long Index, CShapefileColorBreak **pVal)
{
	
	if( Index >= 0 && Index < (int)_allBreaks.size() )
	{	
		if( _allBreaks[Index] != NULL )
			_allBreaks[Index]->AddRef();
		*pVal = _allBreaks[Index];
	}
	else
	{	*pVal = NULL;
		ErrorMessage(tkINDEX_OUT_OF_BOUNDS);	
	}
	return ;
}

void  CShapefileColorScheme::put_ColorBreak(long Index, CShapefileColorBreak *newVal)
{
	if( Index >= 0 && Index < (int)_allBreaks.size() )
	{	
		if (!newVal)
		{
			ErrorMessage(tkUNEXPECTED_NULL_PARAMETER);
		}
		else
		{
			ComHelper::SetRef(newVal, (IDispatch**)&_allBreaks[Index], false);
		}
	}
	else
	{	
		ErrorMessage(tkINDEX_OUT_OF_BOUNDS);
	}
	return ;
}

//***********************************************************************/
//*			ErrorMessage()
//***********************************************************************/
void CShapefileColorScheme::ErrorMessage(long ErrorCode)
{
	_lastErrorCode = ErrorCode;
	CallbackHelper::ErrorMsg("ShapefileColorScheme", _globalCallback, _key, ErrorMsg(_lastErrorCode));
}

void  CShapefileColorScheme::get_LayerHandle(long *pVal)
{

	*pVal = _layerHandle;
	return ;
}

void  CShapefileColorScheme::put_LayerHandle(long newVal)
{
	_layerHandle = newVal;
	return ;
}

void  CShapefileColorScheme::get_FieldIndex(long *pVal)
{
	*pVal = _fieldIndex;
	return ;
}

void  CShapefileColorScheme::put_FieldIndex(long newVal)
{
	_fieldIndex = newVal;
	return ;
}

void  CShapefileColorScheme::get_GlobalCallback(ICallback **pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = _globalCallback;
	if( _globalCallback )
		_globalCallback->AddRef();

	return ;
}

void  CShapefileColorScheme::put_GlobalCallback(ICallback *newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	ComHelper::SetRef(newVal, (IDispatch**)&_globalCallback);
	return ;
}

void  CShapefileColorScheme::get_ErrorMsg(long ErrorCode, BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	USES_CONVERSION;
	*pVal = A2BSTR(ErrorMsg(ErrorCode));

	return ;
}

void  CShapefileColorScheme::get_LastErrorCode(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	*pVal = _lastErrorCode;
	_lastErrorCode = tkNO_ERROR;

	return ;
}

void  CShapefileColorScheme::get_Key(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	USES_CONVERSION;
	*pVal = OLE2BSTR(_key);

	return ;
}

void  CShapefileColorScheme::put_Key(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	::SysFreeString(_key);
	_key = OLE2BSTR(newVal);

	return ;
}

#endif
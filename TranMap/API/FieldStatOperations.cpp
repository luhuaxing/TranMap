// FieldStatOperations.cpp : Implementation of CFieldStatOperations

#include "stdafx.h"
#include "FieldStatOperations.h"
#include "Shapefile.h"

//***********************************************************************
//*		get/put_Key()
//***********************************************************************
void  CFieldStatOperations::get_Key(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	*pVal = OLE2BSTR(_key);
	return ;
}
void  CFieldStatOperations::put_Key(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	::SysFreeString(_key);
	USES_CONVERSION;
	_key = OLE2BSTR(newVal);
	return ;
}

//***********************************************************************/
//*			ErrorMessage()
//***********************************************************************/
void CFieldStatOperations::ErrorMessage(long ErrorCode)
{
	_lastErrorCode = ErrorCode;
	return;
}

void  CFieldStatOperations::get_LastErrorCode(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*pVal = _lastErrorCode;
	_lastErrorCode = tkNO_ERROR;
	return ;
}

void  CFieldStatOperations::get_ErrorMsg(long ErrorCode, BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	*pVal = A2BSTR(ErrorMsg(ErrorCode));
	return ;
}

//****************************************************************
//		get_FieldIndex()
//****************************************************************
void  CFieldStatOperations::get_FieldIndex(int index, int* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if( index < 0 || index >= (long)_operations.size() )
	{	
		ErrorMessage(tkINDEX_OUT_OF_BOUNDS);
		*retVal = -1;
	}
	else
	{
		*retVal = _operations[index]->fieldIndex;
	}
	return ;
};

//****************************************************************
//		get_FieldName()
//****************************************************************
void  CFieldStatOperations::get_FieldName(int index, BSTR* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if( index < 0 || index >= (long)_operations.size() )
	{	
		ErrorMessage(tkINDEX_OUT_OF_BOUNDS);
		*retVal = A2BSTR("");
	}
	else
	{
		*retVal = W2BSTR(_operations[index]->fieldName);
	}
	return ;
};

//****************************************************************
//			get_Operation()
//****************************************************************
void  CFieldStatOperations::get_Operation(int index, tkFieldStatOperation* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if( index < 0 || index >= (long)_operations.size() )
	{	
		ErrorMessage(tkINDEX_OUT_OF_BOUNDS);
		*retVal = (tkFieldStatOperation)0;
	}
	else
	{
		*retVal = _operations[index]->operation;
	}
	return ;
};

//****************************************************************
//			get_Count()
//****************************************************************
void  CFieldStatOperations::get_Count(int* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = _operations.size();
	return ;
};

// ***************************************************************
//		AddFieldIndex()
// ***************************************************************
void  CFieldStatOperations::AddFieldIndex(int fieldIndex, tkFieldStatOperation operation)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	FieldOperation* op = new FieldOperation();
	op->fieldIndex = fieldIndex;
	op->operation = operation;
	op->valid = true;
	op->hasName = false;
	_operations.push_back(op);
	return ;
}

// ***************************************************************
//		AddFieldName()
// ***************************************************************
void  CFieldStatOperations::AddFieldName(BSTR fieldName, tkFieldStatOperation operation)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	FieldOperation* op = new FieldOperation();
	op->fieldName = OLE2W(fieldName);
	op->operation = operation;
	op->valid = true;
	op->hasName = true;
	_operations.push_back(op);
	return ;
}

// ***************************************************************
//		Remove()
// ***************************************************************
void  CFieldStatOperations::Remove(int index, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = VARIANT_FALSE;

	if( index < 0 || index >= (long)_operations.size() )
	{	
		ErrorMessage(tkINDEX_OUT_OF_BOUNDS);
		*retVal = VARIANT_FALSE;
	}
	else
	{
		delete _operations[index];
		_operations.erase(_operations.begin() + index);
		*retVal = VARIANT_TRUE;
	}
	return ;
}

// ***************************************************************
//		Clear()
// ***************************************************************
void  CFieldStatOperations::Clear()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	for(size_t i = 0; i < _operations.size(); i++)
	{
		delete _operations[i];
	}
	_operations.clear();
	return ;
}

//***********************************************************************/
//*			Validate()
//***********************************************************************/
void  CFieldStatOperations::Validate(CShapefile* sf, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = VARIANT_FALSE;
	if (!sf)
	{
		ErrorMessage(tkUNEXPECTED_NULL_PARAMETER);
	}
	else
	{
		long numFields;
		sf->get_NumFields(&numFields);

		for(size_t i = 0; i < _operations.size(); i++)
		{
			FieldOperation* op = _operations[i];
			op->valid = false;
			op->isValidReason = fovValid;

			// searching index for name
			if (op->hasName)
			{
				CTableClass* table = NULL;
				sf->get_Table(&table); 
				CTableClass* table1((CTableClass*)&(*table));
				if (table)
				{
					long fieldIndex;
					CComBSTR bstrName(op->fieldName);
					table1->get_FieldIndexByName(bstrName, &fieldIndex);
					op->fieldIndex = fieldIndex;
				}
			}
			
			CField* field = NULL;
			if (op->fieldIndex >= 0 && op->fieldIndex < numFields)
			{
				sf->get_Field(op->fieldIndex, &field);
				if (field)
				{
					FieldType type;
					field->get_Type(&type);
					op->valid = !((op->operation == fsoSum || op->operation == fsoAvg || op->operation == fsoWeightedAvg) 
									&& type == STRING_FIELD);
					if (op->operation == fsoMode && type != STRING_FIELD)
						op->valid = false;
					delete field;
					op->isValidReason = op->valid ? fovValid : fovNotSupported;
				}
			}
			else
			{
				op->isValidReason = fovFieldNotFound;
			}
		}

		*retVal = VARIANT_TRUE;
		for(size_t i = 0; i < _operations.size(); i++)
		{
			if (!_operations[i]->valid)
			{
				*retVal = VARIANT_FALSE;
				break;
			}
		}
	}
	return ;
}

//****************************************************************
//			get_OperationIsValid
//****************************************************************
void  CFieldStatOperations::get_OperationIsValid(int index, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if( index < 0 || index >= (long)_operations.size() )
	{	
		ErrorMessage(tkINDEX_OUT_OF_BOUNDS);
		*retVal = VARIANT_FALSE;
	}
	else
	{
		*retVal = _operations[index]->valid;
	}
	return ;
}

//****************************************************************
//			get_OperationIsValidReason
//****************************************************************
void  CFieldStatOperations::get_OperationIsValidReason(int index, tkFieldOperationValidity* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if( index < 0 || index >= (long)_operations.size() )
	{	
		ErrorMessage(tkINDEX_OUT_OF_BOUNDS);
		*retVal = fovValid;
	}
	else
	{
		*retVal = _operations[index]->isValidReason;
	}
	return ;
}
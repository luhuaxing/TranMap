// ShapeValidationInfo.cpp : Implementation of CShapeValidationInfo

#include "stdafx.h"
#include "ShapeValidationInfo.h"

// ***********************************************
//		ClassName
// ***********************************************
void  CShapeValidationInfo::get_ClassName(BSTR* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	*retVal = A2BSTR(className);
	return ;
}

// ***********************************************
//		MethodName
// ***********************************************
void  CShapeValidationInfo::get_MethodName(BSTR* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	*retVal = A2BSTR(methodName);
	return ;
}

// ***********************************************
//		ParameterName
// ***********************************************
void  CShapeValidationInfo::get_ParameterName(BSTR* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	*retVal = A2BSTR(parameterName);
	return ;
}

// ***********************************************
//		ValidationType
// ***********************************************
void  CShapeValidationInfo::get_ValidationType(tkShapeValidationType* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = validationType;
	return ;
}

// ***********************************************
//		ValidationMode
// ***********************************************
void  CShapeValidationInfo::get_ValidationMode(tkShapeValidationMode* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = validationMode;
	return ;
}

// ***********************************************
//		IsValid
// ***********************************************
void  CShapeValidationInfo::get_IsValid(VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = (stillInvalidCount > 0 || skippedCount > 0) ? VARIANT_TRUE: VARIANT_FALSE;
	return ;
}

// ***********************************************
//		Status
// ***********************************************
void  CShapeValidationInfo::get_Status(tkShapeValidationStatus* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = validationStatus;
	return ;
}

// ***********************************************
//		WereInvalidCount
// ***********************************************
void  CShapeValidationInfo::get_WereInvalidCount(int* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = wereInvalidCount;
	return ;
}

// ***********************************************
//		StillInvalidCount
// ***********************************************
void  CShapeValidationInfo::get_StillInvalidCount(int* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = stillInvalidCount;
	return ;
}

// ***********************************************
//		FixedCount
// ***********************************************
void  CShapeValidationInfo::get_FixedCount(int* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = wereInvalidCount;
	return ;
}

// ***********************************************
//		SkippedCount
// ***********************************************
void  CShapeValidationInfo::get_SkippedCount(int* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = skippedCount;
	return ;
}

// ***********************************************
//		ErrorCount
// ***********************************************
void  CShapeValidationInfo::get_WasValidated( VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = validationMode != tkShapeValidationMode::NoValidation;
	return ;
}

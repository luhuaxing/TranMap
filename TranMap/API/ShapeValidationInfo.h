// ShapeValidationInfo.h : Declaration of the CShapeValidationInfo

#pragma once

// CShapeValidationInfo
class  CShapeValidationInfo 
{
public:
	CShapeValidationInfo()
	{
		validationType = tkShapeValidationType::svtInput;
		validationMode = tkShapeValidationMode::NoValidation;
		validationStatus = tkShapeValidationStatus::WasntValidated;
		wereInvalidCount = 0;
		fixedCount = 0;
		stillInvalidCount = 0;
		skippedCount = 0;
		gReferenceCounter.AddRef(tkInterface::idShapeValidationInfo);
	}
	~CShapeValidationInfo()
	{
		gReferenceCounter.Release(tkInterface::idShapeValidationInfo);
	}


public:
	void get_ClassName( BSTR* retVal);
	void get_MethodName( BSTR* retVal);
	void get_ParameterName( BSTR* retVal);
	void get_ValidationType( tkShapeValidationType* retVal);
	void get_ValidationMode( tkShapeValidationMode* retVal);
	void get_IsValid( VARIANT_BOOL* retVal);
	void get_Status( tkShapeValidationStatus* retVal);
	void get_WereInvalidCount( int* retVal);
	void get_StillInvalidCount( int* retVal);
	void get_FixedCount( int* retVal);
	void get_SkippedCount( int* retVal);
	void get_WasValidated( VARIANT_BOOL* retVal);

	CString className;
	CString methodName;
	CString parameterName;
	tkShapeValidationType validationType;
	tkShapeValidationMode validationMode;
	tkShapeValidationStatus validationStatus;
	int wereInvalidCount;
	int fixedCount;
	int stillInvalidCount;
	int skippedCount;
};
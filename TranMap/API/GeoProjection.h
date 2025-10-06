#pragma once
#include "GlobalVars.h"
#include "Extents.h"
extern ReferenceCounter gReferenceCounter;

// CGeoProjection
class  CGeoProjection
{
public:
	CGeoProjection()
	{
		USES_CONVERSION;
		_key = A2BSTR("");
		_globalCallback = NULL;
		_lastErrorCode = tkNO_ERROR;
		_projection = (OGRSpatialReference*)OSRNewSpatialReference(NULL);
		_transformation = NULL;	
		gReferenceCounter.AddRef(tkInterface::idGeoProjection);
		_isFrozen = false;
	}

	~CGeoProjection()
	{
		//SysFreeString(_key);
		//StopTransform();
		//if (_projection)
		//{
		//	_projection->Clear();
		//	OGRSpatialReference::DestroySpatialReference(_projection);
		//}
		//gReferenceCounter.Release(tkInterface::idGeoProjection);
	}




public:
	void get_LastErrorCode(/*[out, retval]*/ long *pVal);
	void get_ErrorMsg(/*[in]*/ long ErrorCode, /*[out, retval]*/ BSTR *pVal);
	void get_GlobalCallback(/*[out, retval]*/ ICallback * *pVal);
	void put_GlobalCallback(/*[in]*/ ICallback * newVal);
	void get_Key(/*[out, retval]*/ BSTR *pVal);
	void put_Key(/*[in]*/ BSTR newVal);
	void ImportFromProj4(BSTR proj, VARIANT_BOOL* retVal);
	void ImportFromESRI(BSTR proj, VARIANT_BOOL* retVal);
	void ImportFromEPSG(LONG projCode, VARIANT_BOOL* retVal);
	void ImportFromWKT(BSTR proj, VARIANT_BOOL* retVal);
	void ImportFromAutoDetect(BSTR proj, VARIANT_BOOL* retVal);
	void ExportToProj4(BSTR* retVal);
	void ExportToWKT(BSTR* retVal);
	void SetWellKnownGeogCS(tkCoordinateSystem newVal);
	void get_IsGeographic(VARIANT_BOOL* pVal);
	void get_IsProjected(VARIANT_BOOL* pVal);
	void get_IsLocal(VARIANT_BOOL* pVal);
	void get_IsSame(CGeoProjection* proj, VARIANT_BOOL* pVal);
	void get_IsSameGeogCS(CGeoProjection* proj, VARIANT_BOOL* pVal);
	void get_InverseFlattening(DOUBLE* pVal);
	void get_SemiMajor(DOUBLE* pVal);
	void get_SemiMinor(DOUBLE* pVal);
	void get_ProjectionParam(tkProjectionParameter name, double* value, VARIANT_BOOL* pVal);
	void get_IsEmpty(VARIANT_BOOL* retVal);
	void CopyFrom(CGeoProjection* sourceProj, VARIANT_BOOL* pVal);
	void get_GeogCSParam(tkGeogCSParameter name, DOUBLE* pVal, VARIANT_BOOL* retVal);
	void get_Name(BSTR* pVal);
	void get_ProjectionName(BSTR* pVal);
	void get_GeogCSName(BSTR* pVal);
	void SetGeographicCS(tkCoordinateSystem coordinateSystem);
	void SetWgs84Projection(tkWgs84Projection projection);
	void SetNad83Projection(tkNad83Projection projection);
	void get_IsSameExt(CGeoProjection* proj, CExtents* bounds, int numSamplingPoints, VARIANT_BOOL* pVal);
	void ReadFromFile(BSTR filename, VARIANT_BOOL* retVal);
	void WriteToFile(BSTR filename, VARIANT_BOOL* retVal);
	void StartTransform(CGeoProjection* target, VARIANT_BOOL* retval);
	void Transform(double* x, double* y, VARIANT_BOOL* retval);
	void StopTransform();
	void get_HasTransformation(VARIANT_BOOL* retval);
	void SetGoogleMercator(VARIANT_BOOL* retVal);
	void SetWgs84(VARIANT_BOOL* retVal);
	void Clear(VARIANT_BOOL* retVal);
	void Clone(CGeoProjection** retVal);
	void get_IsFrozen(VARIANT_BOOL* retVal);
	void TryAutoDetectEpsg(int* epsgCode, VARIANT_BOOL* retVal);

private:
	OGRSpatialReference* _projection;
	long _lastErrorCode;
	ICallback * _globalCallback;
	BSTR _key;
	bool _isFrozen;
	OGRCoordinateTransformation* _transformation;

private:
	void ErrorMessage(long ErrorCode);
	bool IsSameProjection(OGRCoordinateTransformation* transf, double x, double y, bool projected);

public:
	OGRSpatialReference* get_SpatialReference() { return _projection; }
	bool get_IsSame(CGeoProjection* proj);
	void SetIsFrozen(bool frozen) {	_isFrozen = frozen; }
	void InjectSpatialReference(OGRSpatialReference* sr);
};
#pragma once
#include "vector.h"
#include "GridColorBreak.h"

// CGridColorScheme
class  CGridColorScheme
{
public:
	CGridColorScheme()
	{
		_globalCallback = NULL;
		_ambientIntensity = 0.7;
		_lightSourceIntensity = 0.7;		
		_lightSourceIntensity = 0.7;
		_lightSourceAzimuth = 90;
		_lightSourceElevation = 45;
		_noDataColor = 0;		
		_lightSource = cppVector(0.0,-0.707,1.0);
		_lastErrorCode = tkNO_ERROR;
		_key = SysAllocString(L"");
		gReferenceCounter.AddRef(tkInterface::idGridColorScheme);
	}

	~CGridColorScheme()
	{
		::SysFreeString(_key);
		Clear();
		gReferenceCounter.Release(tkInterface::idGridColorScheme);
	}


// CGridColorScheme
public:
	void get_Key(/*[out, retval]*/ BSTR *pVal);
	void put_Key(/*[in]*/ BSTR newVal);
	void get_GlobalCallback(/*[out, retval]*/ ICallback * *pVal);
	void put_GlobalCallback(/*[in]*/ ICallback * newVal);
	void get_ErrorMsg(/*[in]*/ long ErrorCode, /*[out, retval]*/ BSTR *pVal);
	void get_LastErrorCode(/*[out, retval]*/ long *pVal);
	void GetLightSource(/*[out,retval]*/ CVector ** result);
	void UsePredefined(/*[in]*/ double LowValue, /*[in]*/ double HighValue, /*[in]*/ PredefinedColorScheme Preset);
	void get_NoDataColor(/*[out, retval]*/ OLE_COLOR *pVal);
	void put_NoDataColor(/*[in]*/ OLE_COLOR newVal);
	void Clear();
	void DeleteBreak(/*[in]*/ long Index);
	void get_Break(/*[in]*/ long Index, /*[out, retval]*/ CGridColorBreak ** pVal);
	void InsertBreak(/*[in]*/ CGridColorBreak * ColorBreak);
	void SetLightSource(/*[in]*/ double Azimuth, /*[in]*/ double Elevation);
	void get_LightSourceElevation(/*[out, retval]*/ double *pVal);
	void get_LightSourceAzimuth(/*[out, retval]*/ double *pVal);
	void get_LightSourceIntensity(/*[out, retval]*/ double *pVal);
	void put_LightSourceIntensity(/*[in]*/ double newVal);
	void get_AmbientIntensity(/*[out, retval]*/ double *pVal);
	void put_AmbientIntensity(/*[in]*/ double newVal);
	void get_NumBreaks(/*[out, retval]*/ long *pVal);
	void InsertAt(/*[in]*/ int Position, /*[in]*/ CGridColorBreak *Break);
	void Serialize(BSTR* retVal);
	void Deserialize(BSTR newVal);
	void ReadFromFile(BSTR mwlegFilename, BSTR nodeName, VARIANT_BOOL* retVal);
	void WriteToFile(BSTR mwlegFilename, BSTR gridName, int bandIndex, VARIANT_BOOL* retVal);
	void ApplyColoringType(ColoringType coloringType);
	void ApplyGradientModel(GradientModel gradientModel);

public:
	CPLXMLNode* CGridColorScheme::SerializeCore(CString ElementName);
	bool CGridColorScheme::DeserializeCore(CPLXMLNode* node);
	void ErrorMessage(long ErrorCode);

private:
	long _lastErrorCode;
	ICallback * _globalCallback;
	BSTR _key;

	std::deque<CGridColorBreak *> _breaks;	//stack of breaks to be used
	OLE_COLOR _noDataColor;				//color to be used when there is no data in the grid
	cppVector _lightSource;	 				//where is the light coming from?
	double _lightSourceIntensity;		//How bright is the light from the lightsource?
										//valid values between 0 and 1, inclusive.
	double _ambientIntensity;			//How bright is the overall lighting in the area?
										//valid values between 0 and 1, inclusive.
	double _lightSourceAzimuth;
	double _lightSourceElevation;
};


#include "stdafx.h"
#include "GlobalVars.h"



#ifdef _DEBUG
CMemLeakDetect gMemLeakDetect;		// comment this line to turn off the memory leaking detection tools	
#endif

// constants to be available  in the whole program
 double pi_ = 3.1415926535897932384626433832795;
 double pi_2 = 1.5707963267948966192313216916398;
 double METERS_PER_DEGREE = 110899.999942;
 double MAX_LAYER_VISIBLE_SCALE = 100000000.0;
 double MERCATOR_MAX_VAL = 20037508.342789244;	// in GMercator for bot long and lat
 double MAX_LATITUDE = 85.05112878;		// in WGS84
 double MAX_LONGITUDE = 180.0;
 int EPSG_AMERSFOORT = 28992;
 double SNAP_TOLERANCE = 20;  // pixels
 double MOUSE_CLICK_TOLERANCE = 20;  // pixels
 double POINT_INSERT_TOLERANCE = 10;  // pixels

// *****************************************************************
//		getGeosHandle
// *****************************************************************
#ifdef GEOS_NEW
GEOSContextHandle_t _geosContextHandle = NULL;
GEOSContextHandle_t getGeosHandle()
{
	if (!_geosContextHandle)
		_geosContextHandle = OGRGeometry::createGEOSContext();
	return _geosContextHandle;
}
#endif

HRESULT CStopExecution::StopFunction(VARIANT_BOOL* retVal)
{
	return S_OK;
}
HRESULT CStopExecution::PromptForMissingFile(BSTR* Filename,  BSTR Msg,  VARIANT_BOOL* Retry,  VARIANT_BOOL* Skip,  VARIANT_BOOL* Abort)
{
	return S_OK;
}

ICallback* GetCallback(){ return m_globalSettings.callback ? m_globalSettings.callback : NULL/*_globalCallback*/; }

// *****************************************************************
//		GetUtils
// *****************************************************************
CUtils* GetUtils()
{
	if (!m_utils)
	{
		m_utils = new CUtils();
	}
	return m_utils;
}

// *****************************************************************
//		GetTempShapefile
// *****************************************************************
CShapefile* GetTempShapefile()
{
	if (!m_shapefile)
	{
		m_shapefile = new CShapefile();
	}
	return m_shapefile;
}
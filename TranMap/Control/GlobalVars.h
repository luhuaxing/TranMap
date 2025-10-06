#pragma once

#pragma warning(disable:4482)	// non-standard extension used - for names of enumerations
#pragma warning(disable:4099)	// missing program database

#include <math.h>
#include <map>
#include <vector>
#include <deque>
#include "gdal_priv.h"
#include "geos_c.h"
#include "ogr_geometry.h"
#include "ogr_spatialref.h"
#include "TranMap.h"
#include "structures.h"
#include "UtilityFunctions.h"
#include "GdalHelper.h"
#include "ReferenceCounter.h"
#include "GlobalSettingsInfo.h"
#include "Logger.h"
#include "Extent.h"
#include "VarH.h"
#include "MapViewCallback.h"
#include "DebugHelper.h"
#include "CallbackHelper.h"
#include "Utils.h"
#include "Shapefile.h"
class CStopExecution 
{
public:
	 HRESULT StopFunction(VARIANT_BOOL* retVal);
	 HRESULT PromptForMissingFile(BSTR* Filename,  BSTR Msg,  VARIANT_BOOL* Retry,  VARIANT_BOOL* Skip,  VARIANT_BOOL* Abort);
};

static GlobalSettingsInfo m_globalSettings;
static ReferenceCounter gReferenceCounter;
static CUtils* m_utils = NULL;
static CShapefile* m_shapefile;
static char* UTF8_ENCODING_MARKER = "utf8";
extern GEOSContextHandle_t _geosContextHandle;

CUtils* GetUtils();
CShapefile* GetTempShapefile();
GEOSContextHandle_t getGeosHandle();

// constants to be available  in the whole program
extern double pi_;
extern double pi_2;
extern double METERS_PER_DEGREE;
extern double MAX_LAYER_VISIBLE_SCALE;
extern double MERCATOR_MAX_VAL;	// in GMercator for bot long and lat
extern double MAX_LATITUDE;		// in WGS84
extern double MAX_LONGITUDE;
extern int EPSG_AMERSFOORT;
extern double SNAP_TOLERANCE;  // pixels
extern double MOUSE_CLICK_TOLERANCE;  // pixels
extern double POINT_INSERT_TOLERANCE;  // pixels

ICallback* GetCallback();

#define MAX_BUFFER 512

#ifdef VLD_FORCE_ENABLE
	#include <vld.h>
#endif

// comment these lines to turn off the memory leaking detection tools
#ifdef _DEBUG
	#include "../debugging/MemLeakDetect.h"
	#pragma warning(disable:4100)
	extern CMemLeakDetect gMemLeakDetect;
#endif

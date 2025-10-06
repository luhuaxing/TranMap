
#pragma once
#include "TileCore.h"
#include "BaseProvider.h"
#include "TileProviders.h"
#include "TileLoader.h"
using namespace std;
#include "PointClass.h"
#include "GlobalVars.h"
extern ReferenceCounter gReferenceCounter;
class CStopExecution;


class  CTiles
{
public:
	CTiles()
	{
		_key = SysAllocString(L"");
		_globalCallback = NULL;
		_lastErrorCode = tkNO_ERROR;
		
		_providers = new CTileProviders();
		((CTileProviders*)_providers)->put_Tiles(this);
		this->SetDefaults();
		_lastZoom = -1;
		_lastProvider = tkTileProvider::ProviderNone;
		gReferenceCounter.AddRef(tkInterface::idTiles);
	}

	~CTiles()
	{
		SysFreeString(_key);
		ClearAll();
		gReferenceCounter.Release(tkInterface::idTiles);
	}
	
	void ClearAll()
	{
		this->Stop();
		this->Clear();
		if (_providers)
		{
			//_providers->Release();
			delete _providers;
			_providers = NULL;
		}
	}

	void SetDefaults()
	{
		m_provider = ((CTileProviders*)_providers)->get_Provider((int)tkTileProvider::OpenStreetMap);
		_visible = true;
		_gridLinesVisible = false;
		_doRamCaching = true;
		_doDiskCaching = false;
		_useRamCache = true;
		_useDiskCache = true;
		_useServer = true;
		_minScaleToCache = 0;
		_maxScaleToCache = 100;
		_projExtentsNeedUpdate = true;
		_scalingRatio = 1.0;
	}

public:
	void get_LastErrorCode(/*[out, retval]*/ long *pVal);
	void get_ErrorMsg(/*[in]*/ long ErrorCode, /*[out, retval]*/ BSTR *pVal);
	void get_GlobalCallback(/*[out, retval]*/ ICallback * *pVal);
	void put_GlobalCallback(/*[in]*/ ICallback * newVal);
	void get_Key(/*[out, retval]*/ BSTR *pVal);
	void put_Key(/*[in]*/ BSTR newVal);
	void get_Visible(VARIANT_BOOL* pVal);
	void put_Visible(VARIANT_BOOL newVal);
	void get_GridLinesVisible(VARIANT_BOOL* pVal);
	void put_GridLinesVisible(VARIANT_BOOL newVal);
	void get_Provider(tkTileProvider* pVal);
	void put_Provider(tkTileProvider newVal);
	void get_DoCaching(tkCacheType type, VARIANT_BOOL* pVal);
	void put_DoCaching(tkCacheType type, VARIANT_BOOL newVal);
	void get_UseCache(tkCacheType type, VARIANT_BOOL* pVal);
	void put_UseCache(tkCacheType type, VARIANT_BOOL newVal);
	void get_UseServer(VARIANT_BOOL* pVal);
	void put_UseServer(VARIANT_BOOL newVal);
	void get_MaxCacheSize(tkCacheType type, double* pVal);
	void put_MaxCacheSize(tkCacheType type, double newVal);
	void get_MinScaleToCache(int* pVal);
	void put_MinScaleToCache(int newVal);
	void get_MaxScaleToCache(int* pVal);
	void put_MaxScaleToCache(int newVal);
	void ClearCache(tkCacheType type);
	void ClearCache2(tkCacheType type, tkTileProvider provider, int fromScale = 0, int toScale = 100);
	void get_CacheSize(tkCacheType type, double* retVal);
	void get_CacheSize2(tkCacheType type, tkTileProvider provider, int scale, double* retVal);
	void Serialize(BSTR* retVal);
	void Deserialize(BSTR newVal);
	void SetProxy(BSTR address, int port, VARIANT_BOOL* retVal);
	void get_Proxy(BSTR* retVal);
	void AutodetectProxy(VARIANT_BOOL* retVal);
	void get_DiskCacheFilename(BSTR* retVal);
	void put_DiskCacheFilename(BSTR pVal);
	void get_Providers(CTileProviders** retVal);
	void get_ProviderId(int* retVal);
	void put_ProviderId(int newVal);
	void GetTilesIndices(CExtents* boundsDegrees, int zoom, int provider, CExtents** retVal);
	void Prefetch(double minLat, double maxLat, double minLng, double maxLng, int zoom, int provider, CStopExecution* stop, LONG* retVal);
	void Prefetch2(int minX, int maxX, int minY, int maxY, int zoom, int provider, CStopExecution* stop, LONG* retVal);
	void get_DiskCacheCount(int provider, int zoom, int xMin, int xMax, int yMin, int yMax, LONG* retVal);
	void get_ProviderName(BSTR* retVal);
	void CheckConnection(BSTR url, VARIANT_BOOL* retVal);
	void GetTileBounds(int provider, int zoom, int tileX, int tileY, CExtents** retVal);
	void get_CurrentZoom(int* retVal);
	void PrefetchToFolder(CExtents* ext, int zoom, int providerId, BSTR savePath, BSTR fileExt, CStopExecution* stop, LONG* retVal);
	void get_PrefetchErrorCount(int* retVal);
	void get_PrefetchTotalCount(int* retVal);
	void ClearPrefetchErrors();
	void StartLogRequests(BSTR filename, VARIANT_BOOL errorsOnly, VARIANT_BOOL* retVal);
	void StopLogRequests();
	void get_LogFilename(BSTR* retVal);
	void get_LogIsOpened(VARIANT_BOOL* retVal);
	void get_LogErrorsOnly(VARIANT_BOOL* retVal);
	void put_LogErrorsOnly(VARIANT_BOOL pVal);
	void get_SleepBeforeRequestTimeout(long* retVal);
	void put_SleepBeforeRequestTimeout(long pVal);
	void get_MaxZoom(int* retVal);
	void get_MinZoom(int* pVal);
	void get_ServerProjection(tkTileProjection* retVal);
	void get_ProjectionStatus(tkTilesProjectionStatus* retVal);
	void get_ScalingRatio(double* pVal);
	void put_ScalingRatio(double newVal);
	void SetProxyAuthentication(BSTR username, BSTR password, BSTR domain, VARIANT_BOOL* retVal);
	void ClearProxyAuthorization();
	void get_ProxyAuthenticationScheme(tkProxyAuthentication* pVal);
	void put_ProxyAuthenticationScheme(tkProxyAuthentication newVal);

private:
	long _lastErrorCode;
	ICallback * _globalCallback;
	BSTR _key;

	double _scalingRatio;

	bool _visible;
	bool _gridLinesVisible;
	bool _doRamCaching;
	bool _doDiskCaching;
	bool _useRamCache;
	bool _useDiskCache;
	bool _useServer;
	
	int _minScaleToCache;
	int _maxScaleToCache;
	
	CTileProviders* _providers;
	
	TileLoader _tileLoader;
	TileLoader _prefetchLoader;
	
	Extent _projExtents;			// extents of the world under current projection; in WGS84 it'll be (-180, 180, -90, 90)
	bool _projExtentsNeedUpdate;	// do we need to update bounds in m_projExtents on the next request?
	CStringW _logPath;
	void* _mapView;

	// to avoid duplicate consecutive requests
	int _lastZoom;
	int _lastProvider;
	Extent _lastTileExtents;	// in tile coordinates
	Extent _lastMapExtents;

public:
	::CCriticalSection m_tilesBufferLock;
	std::vector<TileCore*> m_tiles;
	BaseProvider* m_provider;

private:
	void ErrorMessage(long ErrorCode);
	int ChooseZoom(double xMin, double xMax, double yMin, double yMax, double pixelPerDegree, bool limitByProvider, BaseProvider* provider);
	int ChooseZoom(CExtents* ext, double pixelPerDegree, bool limitByProvider, BaseProvider* provider);
	void getRectangleXY(double xMinD, double xMaxD, double yMinD, double yMaxD, int zoom, CRect &rect, BaseProvider* provider);
	bool ProjectionSupportsWorldWideTransform(CGeoProjection* mapProjection, CGeoProjection* wgs84Projection);
	void SetAuthorization(BSTR userName, BSTR password, BSTR domain);

public:	
	void Init(void* map) {	_mapView = map; }
	void MarkUndrawn();
	long PrefetchCore(int minX, int maxX, int minY, int maxY, int zoom, int providerId, BSTR savePath, BSTR fileExt, CStopExecution* stop);
	void AddTileWithCaching(TileCore* tile);
	void AddTileNoCaching(TileCore* tile);
	void AddTileOnlyCaching(TileCore* tile);
	void AddTileToRamCache(TileCore* tile);
	void Clear();
	void LoadTiles(void* mapView, bool isSnapshot = false, CString key = "");
	void LoadTiles(void* mapView, bool isSnapshot, int providerId, CString key = "");
	bool TilesAreInCache(void* mapView, tkTileProvider providerId = ProviderNone);
	bool TilesAreInScreenBuffer(void* mapView);
	bool GetTilesForMap(void* mapView, int providerId, int& xMin, int& xMax, int& yMin, int& yMax, int& zoom);
	bool GetTilesForMap(void* mapView, int& xMin, int& xMax, int& yMin, int& yMax, int& zoom);
	bool DeserializeCore(CPLXMLNode* node);
	CPLXMLNode* SerializeCore(CString ElementName);
	bool UndrawnTilesExist();
	bool DrawnTilesExist();
	BaseProjection* get_Projection(){return m_provider->Projection;}
	bool ProjectionBounds(BaseProvider* provider, CGeoProjection* mapProjection, CGeoProjection* wgsProjection,tkTransformationMode transformationMode, Extent& retVal);
	void HandleOnTilesLoaded(bool isSnapshot, CString key, bool nothingToLoad);
	void UpdateProjection();
	void Stop();
};

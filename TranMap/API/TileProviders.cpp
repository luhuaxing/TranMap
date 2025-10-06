#include "stdafx.h"
#include "TileProviders.h"
#include "BingMapProvider.h"
#include "OpenStreetMapProvider.h"
#include "OviProvider.h"
#include "GoogleMapProvider.h"
#include "RosreestrProvider.h"
#include "SQLiteCache.h"
#include "RAMCache.h"
#include <atlconv.h>

// ************************************************************
//		get_Provider()
// ************************************************************
BaseProvider* CTileProviders::get_Provider(int providerId)
{
	for (size_t i = 0; i < _providers.size(); i++)
	{
		if (_providers[i]->Id == providerId)
		{
			return _providers[i];
		}
	}
	return NULL;
}

#pragma region "ErrorHandling"
// ************************************************************
//		get_GlobalCallback()
// ************************************************************
void  CTileProviders::get_GlobalCallback(ICallback **pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*pVal = _globalCallback;
	//if( _globalCallback != NULL )
	//	_globalCallback->AddRef();
	return ;
}

// ************************************************************
//		put_GlobalCallback()
// ************************************************************
void  CTileProviders::put_GlobalCallback(ICallback *newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	_globalCallback = newVal;
	return ;
}

// *****************************************************************
//	   get_ErrorMsg()
// *****************************************************************
void  CTileProviders::get_ErrorMsg(long ErrorCode, BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	*pVal = A2BSTR(ErrorMsg(ErrorCode));
	return ;
}

// ************************************************************
//		get_LastErrorCode()
// ************************************************************
void  CTileProviders::get_LastErrorCode(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*pVal = _lastErrorCode;
	_lastErrorCode = tkNO_ERROR;
	return ;
}

// **************************************************************
//		ErrorMessage()
// **************************************************************
void CTileProviders::ErrorMessage(long ErrorCode)
{
	_lastErrorCode = ErrorCode;
	CallbackHelper::ErrorMsg("TileProviders", _globalCallback, _key, ErrorMsg(_lastErrorCode));
}

// ************************************************************
//		get/put_Key()
// ************************************************************
void  CTileProviders::get_Key(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	*pVal = OLE2BSTR(_key);
	return ;
}
void  CTileProviders::put_Key(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	::SysFreeString(_key);
	_key = OLE2BSTR(newVal);
	return ;
}

#pragma endregion

#pragma region Default providers
// *****************************************************
//		getProvider()
// *****************************************************
// Instantiate default providers
BaseProvider* CTileProviders::getProviderCore(tkTileProvider provider )
{
	for (size_t i = 0; i < _providers.size(); i++)
	{
		if (_providers[i]->Id == provider)
			return _providers[i];
	}

	BaseProvider* p = NULL;

	switch (provider)
	{
		case BingMaps:
			p = (BaseProvider*)new BingMapProvider();
			break;
		case BingSatellite:
			p = (BaseProvider*)new BingSatelliteProvider();
			break;
		case BingHybrid:
			p = (BaseProvider*)new BingHybridProvider();
			break;
		case OpenStreetMap:
			p = (BaseProvider*)new OpenStreetMapProvider();
			break;
		case HereMaps:
			p = (BaseProvider*)new OviMapProvider();
			break;
		case HereSatellite:
			p = (BaseProvider*)new OviSatelliteProvider();
			break;
		case HereHybrid:
			p = (BaseProvider*)new OviHybridProvider();
			break;
		case HereTerrain:
			p = (BaseProvider*)new OviTerrainProvider();
			break;
		case GoogleMaps:
			p = (BaseProvider*)new GoogleMapProvider();
			break;
		case GoogleSatellite:
			p = (BaseProvider*)new GoogleSatelliteProvider();
			break;
		case GoogleHybrid:
			p = (BaseProvider*)new GoogleHybridProvider(this);
			break;
		case GoogleTerrain:
			p = (BaseProvider*)new GoogleTerrainProvider();
			break;
		case OpenTransportMap:
			p = (BaseProvider*)new OpenTransportMapProvider();
			break;
		case OpenCycleMap:
			p = (BaseProvider*)new OpenCycleMapProvider();
			break;
		case Rosreestr:
			{
				RosreestrProvider* pr1 = new RosreestrProvider(false);
				pr1->subProviders.push_back(pr1);
				pr1->AddDynamicOverlay(new RosreestrProvider(true));
				pr1->AddDynamicOverlay(new RosreestrBordersProvider());
				p = (BaseProvider*)pr1;
			}
			break;
		case OpenHumanitarianMap:
			p = (BaseProvider*)new OpenHumanitarianMapProvider();
			break;
		case MapQuestAerial:
			p = (BaseProvider*)new MapQuestProvider();
			break;
	}

	if (p) 
	{
		_providers.push_back(p);
	}
	/*else
	{
		CallbackHelper::AssertionFailed(Debug::Format("Provider wasn't found: %d.", (int)provider));
	}*/
	return p;
}
#pragma endregion

// ******************************************************
//		Remove()
// ******************************************************
void  CTileProviders::Remove(LONG providerId, VARIANT_BOOL clearCache, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	BaseProvider* p = this->get_Provider(providerId);
	if (!p)
	{
		ErrorMessage(tkINVALID_PROVIDER_ID);
		return ;
	}
	else
	{
		CustomProvider* custom = dynamic_cast<CustomProvider*>(p);
		if (!custom)
		{
			ErrorMessage(tkCANT_DELETE_DEFAULT_PROVIDER);
			return ;
		}
		else
		{
			if (_tiles != NULL)
			{
				// check, probably the provider is currently in use 
				int id = -1;
				_tiles->get_ProviderId(&id);
				if (p->Id == id)
					_tiles->put_Provider(tkTileProvider::OpenStreetMap);
			}
			
			for (size_t i = 0; i < _providers.size(); i++)
			{
				if (_providers[i]->Id == providerId)
				{
					delete _providers[i];
					_providers.erase(_providers.begin() + i);
					break;
				}
			}

			if (clearCache)
			{
				SQLiteCache::Clear(providerId);
				RamCache::ClearByProvider(providerId);
			}
			return ;
		}
	}
}

// *******************************************************
//			Clear()
// *******************************************************
void  CTileProviders::Clear(VARIANT_BOOL clearCache)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	// make sure no provider is currently in use
	if (_tiles != NULL) {
		tkTileProvider provider;
		_tiles->get_Provider(&provider);
		if(provider == tkTileProvider::ProviderCustom)
			_tiles->put_Provider(tkTileProvider::OpenStreetMap);
	}
	
	// default providers should remain untouched
	std::vector<BaseProvider*>::iterator it = _providers.begin();
	while (it < _providers.end())
	{
		CustomProvider* p = dynamic_cast<CustomProvider*>(*it);
		if (p)
		{
			if (clearCache)
			{
				SQLiteCache::Clear(p->Id);
				RamCache::ClearByProvider(p->Id);
			}

			delete *it;
			it = _providers.erase(it);
		}
		else
		{
			++it;
		}
	}
	return ;
}

// *******************************************************
//			get_Count()
// *******************************************************
void  CTileProviders::get_Count(LONG* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*pVal = _providers.size();
	return ;
}

// *******************************************************
//			Add()
// *******************************************************
void  CTileProviders::Add(int Id, BSTR name, BSTR urlPattern, tkTileProjection projection, int minZoom, int maxZoom, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = VARIANT_FALSE;

	if (minZoom < 1 )
		minZoom = 1;

	if (Id < (int)tkTileProvider::ProviderCustom) {
		this->ErrorMessage(tkPROVIDER_ID_RESERVED);
		return ;
	}

	BaseProvider* p = this->get_Provider(Id);
	if (p) {
		this->ErrorMessage(tkPROVIDER_ID_IN_USE);
		return ;
	}

	try
	{
		USES_CONVERSION;
		CustomProvider* provider = new CustomProvider(Id, OLE2T(name), OLE2T(urlPattern), projection, minZoom, maxZoom);
		_providers.push_back(provider);
		*retVal = VARIANT_TRUE;
	}
	catch(int val)
	{
		switch (val)
		{
			case 1:
				this->ErrorMessage(tkINVALID_PROJECTION);
				break;
			case 2:
				this->ErrorMessage(tkINVALID_URL);
				break;
		}
	}
	return ;
}

#pragma region Provider properties
// *******************************************************
//			get_Id()
// *******************************************************
void  CTileProviders::get_Id(int Index, LONG* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if (Index < 0 || Index >= (int)_providers.size()){
		ErrorMessage(tkINDEX_OUT_OF_BOUNDS);
		*retVal = -1;
	}
	else {
		*retVal = (LONG)_providers[Index]->Id;
	}
	return ;
}

// *******************************************************
//			get_Name()
// *******************************************************
void  CTileProviders::get_Name(int Index, BSTR* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	if (Index < 0 || Index >= (int)_providers.size())
	{
		ErrorMessage(tkINDEX_OUT_OF_BOUNDS);
		*retVal = A2BSTR("");
	}
	else {
		*retVal = A2BSTR(_providers[Index]->Name);
	}	
	return ;
}

void  CTileProviders::put_Name(int Index, BSTR pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if (Index < 0 || Index >= (int)_providers.size())
	{
		ErrorMessage(tkINDEX_OUT_OF_BOUNDS);
		return ;
	}
	else
	{
		USES_CONVERSION;
		_providers[Index]->Name = OLE2A(pVal);
	}
	return ;
}

// *******************************************************
//			Language()
// *******************************************************
void  CTileProviders::get_Language(int Index, BSTR* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	if (Index < 0 || Index >= (int)_providers.size())
	{
		ErrorMessage(tkINDEX_OUT_OF_BOUNDS);
		*retVal = A2BSTR("");
	}
	else {
		*retVal = A2BSTR(_providers[Index]->LanguageStr);
	}	
	return ;
}

void  CTileProviders::put_Language(int Index, BSTR pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if (Index < 0 || Index >= (int)_providers.size())
	{
		ErrorMessage(tkINDEX_OUT_OF_BOUNDS);
	}
	else
	{
		USES_CONVERSION;
		_providers[Index]->LanguageStr = OLE2A(pVal);
	}
	return ;
}

// ******************************************************
//		get_IsCustom()
// ******************************************************
void  CTileProviders::get_IsCustom(int Index, VARIANT_BOOL* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	return ;
}

// *******************************************************
//			get_Version()
// *******************************************************
void  CTileProviders::get_Version(int Index, BSTR* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	if (Index < 0 || Index >= (int)_providers.size())
	{
		ErrorMessage(tkINDEX_OUT_OF_BOUNDS);
		*retVal = A2BSTR("");
	}
	else {
		*retVal = A2BSTR(_providers[Index]->Version);
	}	
	return ;
}

// *******************************************************
//			put_Version()
// *******************************************************
void  CTileProviders::put_Version(int Index, BSTR pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if (Index < 0 || Index >= (int)_providers.size())
	{
		ErrorMessage(tkINDEX_OUT_OF_BOUNDS);
	}
	else
	{
		USES_CONVERSION;
		_providers[Index]->Version = OLE2A(pVal);
	}
	return ;
}

// *******************************************************
//			get_UrlPattern()
// *******************************************************
void  CTileProviders::get_UrlPattern(int Index, BSTR* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if (Index < 0 || Index >= (int)_providers.size())
	{
		ErrorMessage(tkINDEX_OUT_OF_BOUNDS);
		*retVal = A2BSTR("");
	}
	else {
		*retVal = A2BSTR(_providers[Index]->UrlFormat);
	}	
	return ;
}

// *******************************************************
//			get_Projection()
// *******************************************************
void  CTileProviders::get_Projection(int Index, tkTileProjection* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if (Index < 0 || Index >= (int)_providers.size())
	{
		ErrorMessage(tkINDEX_OUT_OF_BOUNDS);
		*retVal = (tkTileProjection)-1;
	}
	else {
		CustomProvider* p = dynamic_cast<CustomProvider*>(_providers[Index]);
		*retVal = p ? p->m_projectionId :(tkTileProjection)-1;
	}	
	return ;
}

// *******************************************************
//			get_MinZoom()
// *******************************************************
void  CTileProviders::get_MinZoom(int Index, int* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if (Index < 0 || Index >= (int)_providers.size())
	{
		ErrorMessage(tkINDEX_OUT_OF_BOUNDS);
		*retVal = -1;
	}
	else {
		*retVal = _providers[Index]->minZoom;
	}	
	return ;
}

// *******************************************************
//			get_MaxZoom()
// *******************************************************
void  CTileProviders::get_MaxZoom(int Index, int* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if (Index < 0 || Index >= (int)_providers.size())
	{
		ErrorMessage(tkINDEX_OUT_OF_BOUNDS);
		*retVal = -1;
	}
	else {
		*retVal = _providers[Index]->maxZoom;
	}	
	return ;
}
#pragma endregion

// *******************************************************
//			get_IndexByProvider()
// *******************************************************
void  CTileProviders::get_IndexByProvider(tkTileProvider provider, int* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	this->get_IndexByProviderId((int)provider, retVal);
	return ;
}

void  CTileProviders::get_IndexByProviderId(int providerId, int* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = -1;
	for (size_t i = 0; i < _providers.size(); i++)
	{
		if (providerId == _providers[i]->Id)
		{
			*retVal = i;
			break;
		}
	}
	return ;
}

// *******************************************************
//			get_CopyrightNotice()
// *******************************************************
CStringW CTileProviders::get_CopyrightNotice(tkTileProvider provider)
{
	int index = -1;
	get_IndexByProviderId((int)provider, &index);
	if (index >= 0 && index < (int)_providers.size()) {
		return _providers[index]->GetCopyright();
	}
	return L"";
}

// *******************************************************
//			get_LicenseUrl()
// *******************************************************
CString CTileProviders::get_LicenseUrl(tkTileProvider provider)
{
	int index = -1;
	get_IndexByProviderId((int)provider, &index);
	if (index >= 0 && index < (int)_providers.size()) {
		return _providers[index]->LicenseUrl;
	}
	return "";
}



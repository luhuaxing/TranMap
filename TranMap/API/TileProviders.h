
#pragma once
#include "CustomProvider.h"
#include "Tiles.h"
class CTiles;


#define TILE_PROVIDER_COUNT 23

class  CTileProviders
{
public:
	CTileProviders()
		: _tiles(NULL)
	{
		_key = SysAllocString(L"");
		_globalCallback = NULL;
		_lastErrorCode = tkNO_ERROR;
		for (int i = 0; i <= TILE_PROVIDER_COUNT; i++)
		{
			// we don't care about the value returned; a provider is added to the list, that's all needed
			getProviderCore((tkTileProvider)i);
		}
	}
	~CTileProviders()
	{
		SysFreeString(_key);
		for (size_t i = 0; i < _providers.size(); i++) {
			_providers[i]->ClearSubProviders();
		}
		for (size_t i = 0; i < _providers.size(); i++)	{
			delete _providers[i];
		}
	}

	
public:
	void get_LastErrorCode(/*[out, retval]*/ long *pVal);
	void get_ErrorMsg(/*[in]*/ long ErrorCode, /*[out, retval]*/ BSTR *pVal);
	void get_GlobalCallback(/*[out, retval]*/ ICallback * *pVal);
	void put_GlobalCallback(/*[in]*/ ICallback * newVal);
	void get_Key(/*[out, retval]*/ BSTR *pVal);
	void put_Key(/*[in]*/ BSTR newVal);
	
	void Remove(LONG provider, VARIANT_BOOL clearCache, VARIANT_BOOL* retVal);
	void Clear(VARIANT_BOOL clearCache);
	void get_Count(LONG* pVal);
	void Add(int Id, BSTR name, BSTR urlPattern, tkTileProjection projection, int minZoom, int maxZoom, VARIANT_BOOL* retVal);
	
	void get_Id(int Index, LONG* retVal);
	void get_Name(int Index, BSTR* retVal);
	void put_Name(int Index, BSTR pVal);
	void get_UrlPattern(int Index, BSTR* retVal);
	void get_Projection(int Index, tkTileProjection* retVal);
	void get_MinZoom(int Index, int* retVal);
	void get_MaxZoom(int Index, int* retVal);
	void get_IsCustom(int Index, VARIANT_BOOL* retVal);
	void get_Version(int Index, BSTR* retVal);
	void put_Version(int Index, BSTR newVal);
	void get_IndexByProvider(tkTileProvider provider, int* retVal);
	void get_IndexByProviderId(int provider, int* retVal);
	void get_Language(int Index, BSTR* retVal);
	void put_Language(int Index, BSTR twoLetterCode);

private:	
	CTiles* _tiles;	// reference the parent
	vector<BaseProvider*> _providers;
	long _lastErrorCode;
	ICallback * _globalCallback;
	BSTR _key;

private:
	void ErrorMessage(long ErrorCode);
	BaseProvider* getProviderCore(tkTileProvider providerId);

public:
	vector<BaseProvider*>* GetList() { return &_providers;	}
	void put_Tiles(CTiles* tiles){	_tiles = tiles;}
	BaseProvider* get_Provider(int providerId);
	CStringW get_CopyrightNotice(tkTileProvider provider);
	CString get_LicenseUrl(tkTileProvider provider);
};


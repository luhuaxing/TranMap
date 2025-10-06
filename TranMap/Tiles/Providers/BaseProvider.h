
#pragma once
#include "geopoint.h"
#include "baseprojection.h"
#include "TileCore.h"
#include <atlhttp.h>		// ATL HTTP Client (CAtlHttpClient)
#include "afxmt.h"

extern Debug::Logger tilesLogger;

class HttpClientEx: public CAtlHttpClient
{
public:
	bool _inUse;

	HttpClientEx()
	{
		_inUse = false;
	}

	void SetDefaultUrl(CString url)
	{
		CAtlHttpClient::SetDefaultUrl(url);
	}
};

class BasicAuth : public CBasicAuthObject, public ATL::IAuthInfo
{
private:
	CString _username; 
	CString _password;
	CString _domain;
public:
	void SetCredentials(CString username, CString password, CString domain)
	{
		_username = username;
		_password = password;
		_domain = domain;
	}

	void Init(CAtlHttpClient *pSocket, IAuthInfo *pAuthInfo)
	{
		CBasicAuthObject::Init(pSocket, pAuthInfo);
	}

	bool Authenticate(LPCTSTR szAuthTypes, bool bProxy)
	{
		bool result = CBasicAuthObject::Authenticate(szAuthTypes, bProxy);
		if (!result) {
			CallbackHelper::ErrorMsg("Tiles proxy authentication failed. Check if proper credentials are set for Tiles.SetProxyAuthentication.");
		}
		return result;
	}

	HRESULT GetPassword(LPTSTR szPwd, DWORD* dwBuffSize)
	{
		if (CopyCString(_password, szPwd, dwBuffSize))
			return S_OK;
		return E_FAIL;
	}

	HRESULT GetUsername(LPTSTR szUid, DWORD* dwBuffSize)
	{
		if (CopyCString(_username, szUid, dwBuffSize))
			return S_OK;
		return E_FAIL;
	}
	HRESULT GetDomain(LPTSTR szDomain, DWORD* dwBuffSize)
	{
		// MS sample indicates that it's not expected to be called at all
		// http_://msdn.microsoft.com/en-us/library/f3wxbf3f%28v=vs.80%29.aspx
		return S_OK;
	}
};

// Downloads map tiles via HTTP; this is abstract class to inherit from
class BaseProvider
{
protected:
	static CString m_proxyAddress;
	static short m_proxyPort;
	static CString _proxyUsername;
	static CString _proxyPassword;
	static CString _proxyDomain;
	std::vector<HttpClientEx*> _httpClients;
	static ::CCriticalSection _clientLock;
	int _initAttemptCount;
public:
	bool BaseProvider::CheckConnection(CString url);
	std::vector<BaseProvider*> subProviders;	// for complex providers with more than one source bitmap per tile
	BaseProjection* Projection;
	void *mapView;
	int Id;
	CString LanguageStr;
	CString RefererUrl;
	CString LicenseUrl;
	CStringW Copyright;
	CString ServerLetters;
	CString Name;
	CString UrlFormat;
	CString Version;
	CSize minOfTiles;
	CSize maxOfTiles;
	int minZoom;
    int maxZoom;
	int zoom;
	bool Selected;	// is used by clients only
	int httpStatus;
	bool IsStopped;
	bool DynamicOverlay;

private:
	CMemoryBitmap* GetTileImageUsingHttp(CString urlStr, CString shortUrl, bool recursive = false);

protected:
	virtual CString MakeTileImageUrl(CPoint &pos, int zoom) = 0;

	int GetServerNum(CPoint&pos, int max)
	{
		return (pos.x + 2 * pos.y) % max;
	}
public:
	BaseProvider()
		: minZoom(1), maxZoom(18), zoom(0), httpStatus(200)
	{
		mapView = NULL;
		LanguageStr = "en";
		Projection = NULL;
		Selected = false;
		IsStopped = false;
		DynamicOverlay = false;
		_initAttemptCount = 0;
		LicenseUrl = "https://mapwingis.codeplex.com/wikipage?title=tiles";
	}

	virtual ~BaseProvider(void)
	{
		if (Projection != NULL)
			delete Projection;
		for(size_t i = 0; i < _httpClients.size(); i++)
		{
			_httpClients[i]->Close();
			delete _httpClients[i];
		}
		_httpClients.clear();
	};

	// proxy support
	short get_ProxyPort() {return m_proxyPort;}
	CString get_ProxyAddress() {return m_proxyAddress;}
	bool SetProxy(CString address, int port);
	bool SetProxyAuthorization(CString username, CString password, CString domain);
	void ClearProxyAuthorization();
	bool AutodetectProxy();
	void AddDynamicOverlay(BaseProvider* p);
	void ClearSubProviders();
	virtual CStringW GetCopyright() {
		return Copyright;
	}
	CMemoryBitmap* DownloadBitmap(CPoint &pos, int zoom);
	TileCore* GetTileImage(CPoint &pos, int zoom);	
	virtual bool Initialize() { return true; };
	bool InitHttpClient(HttpClientEx& httpClient, BasicAuth& basicAuth, CNTLMAuthObject& ntlmAuth);
};
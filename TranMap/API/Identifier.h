// Identifier.h : Declaration of the CIdentifier
#pragma once

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;

// CIdentifier
class  CIdentifier
{
public:
	CIdentifier()
	{
		_hotTracking = VARIANT_TRUE;
		_mode = imAllLayers;
		_color = RGB(255, 0, 0); //RGB(30, 144, 255);
		_activeLayer = -1;
	}

public:
	void get_HotTracking(VARIANT_BOOL* pVal);
	void put_HotTracking(VARIANT_BOOL newVal);
	void get_IdentifierMode(tkIdentifierMode* pVal);
	void put_IdentifierMode(tkIdentifierMode newVal);
	void get_OutlineColor(OLE_COLOR* pVal);
	void put_OutlineColor(OLE_COLOR newVal);
	void get_ActiveLayer(LONG* pVal);
	void put_ActiveLayer(LONG newVal);

private:
	VARIANT_BOOL _hotTracking;
	tkIdentifierMode _mode;
	OLE_COLOR _color;
	LONG _activeLayer;
};
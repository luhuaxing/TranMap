// ********************************************************************************************************
// File name: Map.cpp
// Description: Implementation of the CMapView ActiveX Control class.
// ********************************************************************************************************
// The contents of this file are subject to the Mozilla Public License Version 1.1 (the "License"); 
// you may not use this file except in compliance with the License. You may obtain a copy of the License at 
// http://www.mozilla.org/MPL/ 
// Software distributed under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF 
// ANY KIND, either express or implied. See the License for the specific language governing rights and 
// limitations under the License. 
//
// The Original Code is MapWindow Open Source. 
//
// The Initial Developer of this version of the Original Code is Daniel P. Ames using portions created by 
// Utah State University and the Idaho National Engineering and Environmental Lab that were released as 
// public domain in March 2004.  
//
// Contributor(s): (Open source contributors should list themselves and their modifications here). 
// ----------------------------------------------------------------------------------------
// dpa, ah, Chris Michaelis (cdm), Stan, Tom Shanley (tws), Andrew Purkis (ajp), Ray Quay,
// Rob Cairns, Sergei Leschinski (lsu), Neio, sm, 
// ----------------------------------------------------------------------------------------

#include "stdafx.h"
#include "Map.h"

#include "Measuring.h"
#include "Tiles.h"
#include "ShapeEditor.h"
#include "UndoList.h"
using namespace std;

//disable some known warnings we don't care about
#pragma warning(disable:4244)
#pragma warning(disable:4018)
#pragma warning(disable:4800)
#pragma warning(disable:4554)
#pragma warning(disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma region Control info
IMPLEMENT_DYNCREATE(CMapView, CView)



#pragma region Constructor/destructor

// ********************************************************************
//		CMapView() constructor
// ********************************************************************
CMapView::CMapView() 
	: _vals("AZ0CY1EX2GV3IT4KR5MP6ON7QL8SJ9UH0WF1DB2"), 
	_valsLen(39), _isSnapshot(false),
	_brushBlue(Gdiplus::Color::Blue),
	_brushBlack(Gdiplus::Color::Black), 
	_brushWhite(Gdiplus::Color::White), 
	_brushLightGray(Gdiplus::Color::LightGray),
	_penGray(Gdiplus::Color::Gray),
	_brushGray(Gdiplus::Color::Gray), 
	_penDarkGray(Gdiplus::Color::DarkSlateGray),
	_bufferBitmap(NULL),
	_tilesBitmap(NULL),
	_layerBitmap(NULL),
	_drawingBitmap(NULL),
	_tempBitmap(NULL),
	_moveBitmap(NULL),
	_volatileBitmap(NULL)
{
	//this->GdiplusStartup();	       // commented intentionally; moved to ExitInstance; see comments below
	Startup();
	SetDefaults();
	this->SetTileProvider(tkTileProvider::OpenHumanitarianMap);
}

// ****************************************************
//	    Destructor
// ****************************************************
CMapView::~CMapView()
{
	LockWindow(lmLock);   // we don't need any more redraws

	this->RemoveAllLayers();

	this->ClearDrawings();

	ReleaseTempObjects();
	
	this->Shutdown();

	//this->GdiplusShutdown();      // commented intentionally; moved to ExitInstance; see comments below
}

// **********************************************************************
//	Clear
// **********************************************************************
void CMapView::Clear()
{
	this->RemoveAllLayers();
	this->ClearDrawings();
	ReleaseTempObjects();
	SetDefaults();

	CGeoProjection* p = new CGeoProjection();
	SetGeoProjection(p);

	Redraw();
}

// **********************************************************************
//	Startup
// **********************************************************************
// Must be called from constructor only
void CMapView::Startup()
{

	Utility::InitGdiPlusFont(&_fontCourier, L"Courier New", 9.0f);
	Utility::InitGdiPlusFont(&_fontArial, L"Arial", 9.0f);
	USES_CONVERSION;
	//lu
	Utility::InitGdiPlusFont(&_fontCourierSmall, L"Courier New", 8.0f);
	Utility::InitGdiPlusFont(&_fontCourierLink, L"Courier New", 8.0f);
/*	_fontCourierSmall = new Gdiplus::Font(A2W("Courier New"), 8.0);
	_fontCourierLink = new Gdiplus::Font(A2W("Courier New"), 8.0);*/ 

	m_mapCursor = crsrMapDefault;
	_interactiveLayerHandle = -1;			// TODO: remove (currently not used)
	_panningAnimation = false;
	_zoombarTargetZoom = -1;
	_dragging.Move = CPoint(0,0);
	_dragging.Start = CPoint(0,0);
	_dragging.Operation = DragNone;
	_currentZoom = -1;
	_spacePressed = false;
	_lastRedrawTime = 0.0f;
	_projectionChangeCount = 0;
	_tileBuffer.Extents.left = 0.0;
	_tileBuffer.Extents.right = 0.0;
	_tileBuffer.Extents.top = 0.0;
	_tileBuffer.Extents.bottom = 0.0;
	_tileBuffer.Zoom = -1;
	_tileBuffer.Provider = (int)tkTileProvider::ProviderNone;
	_tileBuffer.Initialized = false;
	_tileProjectionState = ProjectionDoTransform;

	_globalCallback = NULL;

	m_key = "";

	_viewHeight = 0;
	_viewWidth = 0;

	//Cursors
	_cursorPan = AfxGetApp()->LoadCursor(IDC_PAN);
	_cursorZoomin = AfxGetApp()->LoadCursor(IDC_ZOOMIN);
	_cursorZoomout = AfxGetApp()->LoadCursor(IDC_ZOOMOUT);
	_cursorSelect = AfxGetApp()->LoadCursor(IDC_SELECT);
	_cursorMeasure = AfxGetApp()->LoadCursor(IDC_MEASURE);
	_cursorDigitize = AfxGetApp()->LoadCursor(IDC_DIGITIZE_CURSOR);
	_cursorVertex = AfxGetApp()->LoadCursor(IDC_VERTEX_CURSOR);
	_cursorRotate = AfxGetApp()->LoadCursor(IDC_ROTATE_CURSOR);
	_cursorMove = AfxGetApp()->LoadCursor(IDC_MOVE_CURSOR);
	_cursorSelect2 = AfxGetApp()->LoadCursor(IDC_SELECT2_CURSOR);
	_cursorIdentify = AfxGetApp()->LoadCursor(IDC_IDENTIFY_CURSOR);

	_udCursor = NULL;

	_ttipCtrl = new CButton();
	m_drawMutex.Unlock();
	_mapstateMutex.Unlock();
	_setmaxstdio(2048);
	srand (time(NULL));

	_imageGroups = NULL;
	_hotTracking.Shapefile = NULL;
	_rotate = NULL;

	_tiles = new CTiles();
	((CTiles*)_tiles)->Init((void*)this);

	_measuring = new CMeasuring();
	_shapeEditor = new CShapeEditor();
	_geodesicShape =  new CShapeEditor();

	_identifier = new CIdentifier();
	_fileManager = new CFileManager();
	_undoList = new CUndoList();
	((CUndoList*)_undoList)->SetMapCallback((IMapViewCallback*)this);

	InitProjections();

	// let them all work by default 
	_customDrawingFlags = (tkCustomDrawingFlags)(BeforeAfterDrawing | OnDrawBackBufferBitmapData | OnDrawBackBufferHdc);

	GetMeasuringBase()->SetMapCallback((IMapViewCallback*)this, ShapeInputMode::simMeasuring);
	_shapeEditor->SetMapCallback((IMapViewCallback*)this);
	_geodesicShape->SetMapCallback((IMapViewCallback*)this);
}

// **********************************************************************
//	SetDefaults
// **********************************************************************
void CMapView::SetDefaults()
{
	// temp state variables
	_mouseTracking = false;
	_copyrightLinkActive = FALSE;
	_copyrightRect = Gdiplus::RectF(0.0F, 0.0F, 0.0F, 0.0F);
	_shapeCountInView = 0;
	_currentDrawing = -1;
	_rectTrackerIsActive = false;
	_lastWidthMeters = 0.0;
	_redrawId = 0;
	_lastZooombarPart = ZoombarNone;
	_hotTracking.LayerHandle = -1;
	_hotTracking.ShapeIndex = -1;
	_isSizing = false;
	_showingToolTip = FALSE;
	_lockCount = 0;
	_canUseLayerBuffer = FALSE;
	_canUseVolatileBuffer = FALSE;
	_canUseMainBuffer = false;
	_leftButtonDown = FALSE;
	_lastCursorMode = cmNone;
	_reverseZooming = false;
	_activeLayerPosition = 0;
	_knownExtents = keNone;
	_measuringPersistent = false;
	_lastErrorCode = tkNO_ERROR;
	// public control properties
	m_sendMouseMove = FALSE;
	m_sendMouseDown = FALSE;
	m_sendMouseUp = FALSE;
	m_sendSelectBoxDrag = FALSE;
	m_sendSelectBoxFinal = FALSE;
	m_sendOnDrawBackBuffer = FALSE;
	m_extentHistory = 20;
	m_doubleBuffer = TRUE;
	m_zoomPercent = 0.3;
	m_cursorMode = tkCursorMode::cmZoomIn;
	m_backColor = RGB( 255, 255, 255 );
	m_extentPad = 0.02;
	_rotateAngle = 0.0f;
	_canUseImageGrouping = VARIANT_FALSE;
	_grabProjectionFromData = VARIANT_TRUE;
	_hasHotTracking = false;
	_showCoordinates = cdmAuto;
	_zoomBehavior = zbUseTileLevels;
	_scalebarVisible = VARIANT_TRUE;
	_zoombarVisible = VARIANT_TRUE;
	_multilineLabeling = true;
	_mapResizeBehavior = rbClassic;
	_doTrapRMouseDown = TRUE;
	_useSeamlessPan = FALSE;
	_mouseWheelSpeed = 0.5;
	// lu _shapeDrawingMethod = dmNewSymbology;
	_unitsOfMeasure = umMeters;
	_disableWaitCursor = false;
	_lineSeparationFactor = 3;		
	_useLabelCollision = false;
	_showRedrawTime = VARIANT_FALSE;
	_showVersionNumber = VARIANT_FALSE;	
	_scalebarUnits = tkScalebarUnits::GoogleStyle;
	_zoomBarVerbosity = tkZoomBarVerbosity::zbvFull;
	_panningInertia = csFalse;
	_reuseTileBuffer = TRUE;
	_zoomAnimation = csAuto;
	_zoomBoxStyle = tkZoomBoxStyle::zbsBlue;
	_identifierMode = imAllLayers;
	_zoomBarMinZoom = -1;
	_zoomBarMaxZoom = -1;
	

	// TODO: perhaps it's better to grab those from property exchanged (i.e. reverting only runtime changes)
	// perhaps this call can do this:
	//RequestAsynchronousExchange(GetPropertyExchangeVersion());

	((CTiles*)_tiles)->SetDefaults();
	GetMeasuringBase()->Clear();
}

// **********************************************************************
//	ReleaseTempObjects
// **********************************************************************
void CMapView::ReleaseTempObjects()
{	
	_collisionList.Clear();

	_dragging.CloseShapefile();

	_rotateAngle = 0.0f;
	if (_rotate)
	{
		delete _rotate;
		_rotate = NULL;
	}

	if (_imageGroups)
	{
		for (int i = 0; i < _imageGroups->size(); i++)
		{
			delete (*_imageGroups)[i];
		}
		_imageGroups->clear();
		delete _imageGroups;
		_imageGroups = NULL;
	}
}


// **********************************************************************
//	Shutdown
// **********************************************************************
// Must be called from destructor only
void CMapView::Shutdown()
{
	Utility::ClosePointer(&_fontCourier);
	Utility::ClosePointer(&_fontCourierSmall);
	Utility::ClosePointer(&_fontCourierLink);
	Utility::ClosePointer(&_fontArial);
	
	((CTiles*)_tiles)->Stop();

	if( _globalCallback )
		delete _globalCallback;

	Utility::ClosePointer(&_bufferBitmap);
	Utility::ClosePointer(&_tilesBitmap);
	Utility::ClosePointer(&_drawingBitmap);
	Utility::ClosePointer(&_volatileBitmap);
	Utility::ClosePointer(&_moveBitmap);
	Utility::ClosePointer(&_tempBitmap);
	Utility::ClosePointer(&_layerBitmap);

	ClearPanningList();

	ReleaseProjections();

	if (_identifier)
		delete _identifier;

	if (_fileManager)
		delete _fileManager;

	if (_measuring)
		delete _measuring;

	if (_shapeEditor)
		delete _shapeEditor;

	if (_geodesicShape)
		delete _geodesicShape;
	
	if (_undoList)
		delete _undoList;

	if (_tiles)
	{
		((CTiles*)_tiles)->ClearAll();
		delete _tiles;
	}

	delete _ttipCtrl;
}

// ********************************************************************
//		OnCreate()
// ********************************************************************
int CMapView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;	

	DragAcceptFiles( TRUE );

	CRect rect(0,0,0,0);
	_ttipCtrl->Create(NULL,WS_CHILD,rect,this,IDC_TTBTN);
	_ttipCtrl->ShowWindow(FALSE);

	//CTool Tip
	_ttip.Create(this,TTS_ALWAYSTIP);
	_ttip.Activate(TRUE);
	_ttip.AddTool(this,"",rect,IDC_TTBTN);
	_ttip.SetDelayTime(TTDT_AUTOPOP,0);
	_ttip.SetDelayTime(TTDT_INITIAL,0);
	_ttip.SetDelayTime(TTDT_RESHOW,0);

	return 0;
}
#pragma endregion

#pragma region Property exchange

// **********************************************************************
//	GetPropertyExchangeVersion
// **********************************************************************
DWORD CMapView::GetPropertyExchangeVersion()
{
	// properties can be added between versions, so let use a bit different numbering
	//return MAKELONG(_wVerMinor, _wVerMajor);
	return MAKELONG(49, 11);
}

//// **********************************************************************
////	CMapView::DoPropExchange
//// **********************************************************************
//// Persistence support: http://msdn.microsoft.com/en-us/library/xxf9wx2c.aspx
//void CMapView::DoPropExchange(CPropExchange* pPX)
//{
//	try
//	{
//		bool loading = pPX->IsLoading();
//
//		PX_Color( pPX, "BackColor", m_backColor, RGB( 255, 255, 255 ) );
//		PX_Double( pPX, "ExtentPad", m_extentPad, .02 );
//		PX_Short( pPX, "CursorMode", m_cursorMode, 0 );
//		PX_Double( pPX, "ZoomPercent", m_zoomPercent, .3 );
//		PX_Bool( pPX, "SendMouseMove", m_sendMouseMove, FALSE );
//		PX_Bool( pPX, "SendMouseDown", m_sendMouseDown, FALSE );
//		PX_Bool( pPX, "SendMouseUp", m_sendMouseUp, FALSE );
//		PX_Bool( pPX, "SendSelectBoxDrag", m_sendSelectBoxDrag, FALSE );
//		PX_Bool( pPX, "SendSelectBoxFinal", m_sendSelectBoxFinal, FALSE );
//		PX_Long( pPX, "ExtentHistory", m_extentHistory, 20 );
//		PX_Bool( pPX, "DoubleBuffer", m_doubleBuffer, TRUE );
//		PX_Bool( pPX, "SendOnDrawBackBuffer", m_sendOnDrawBackBuffer, FALSE);
//		PX_Bool( pPX, "ShowRedrawTime", _showRedrawTime, FALSE);
//		PX_Bool( pPX, "ShowVersionNumber", _showVersionNumber, FALSE);
//		PX_Double( pPX, "MouseWheelSpeed", _mouseWheelSpeed, .5 );
//		PX_Bool( pPX, "UseSeamlessPan", _useSeamlessPan, FALSE );
//		PX_Bool( pPX, "MultilineLabels", _multilineLabeling, FALSE );
//		PX_Bool( pPX, "TrapRMouseDown", _doTrapRMouseDown, TRUE );
//		PX_Bool( pPX, "DisableWaitCursor", _disableWaitCursor, FALSE );	
//		PX_Float( pPX, "MapRotationAngle", _rotateAngle, 0.0 );
//		PX_Bool( pPX, "CanUseImageGrouping", _canUseImageGrouping, FALSE );
//		PX_Bool( pPX, "ScalebarVisible", _scalebarVisible, TRUE );
//		
//		// enumerated constants aren't supported directly so temp buffer will be used
//		// we don't care about the direction of exchange, doing both getting and setting
//		long temp;
//		temp = (long)_mapResizeBehavior;
//		PX_Long( pPX, "MapResizeBehavior", temp, 0 );	//rbClassic
//		_mapResizeBehavior = (tkResizeBehavior)temp;
//
//		temp = (long)_zoomBehavior;
//		PX_Long( pPX, "ZoomBehavior", temp, 1 );	// zbUseTileLevels
//		_zoomBehavior = (tkZoomBehavior)temp;
//
//		temp = (long)_scalebarUnits;
//		PX_Long( pPX, "ScalebarUnits", temp, 2 );	//suGoogleStyle
//		_scalebarUnits = (tkScalebarUnits)temp;
//
//		temp = (long)_shapeDrawingMethod;
//		PX_Long( pPX, "ShapeDrawingMethod", temp, 3 );	// dmNewSymbology
//		_shapeDrawingMethod = (tkShapeDrawingMethod)temp;
//
//		temp = (long)_unitsOfMeasure;
//		PX_Long( pPX, "MapUnits", temp, 6 );			//umMeters
//		_unitsOfMeasure = (tkUnitsOfMeasure)temp;
//
//		temp = (long)_showCoordinates;
//		PX_Long( pPX, "ShowCoordinates", temp, 1 );			//cdmAuto
//		_showCoordinates = (tkCoordinatesDisplay)temp;
//
//		temp = (long)GetTileProvider();
//		PX_Long( pPX, "TileProvider", temp, 0 );		// OpenStreetMap
//		SetTileProvider((tkTileProvider)temp);
//
//		tkMapProjection projection;
//		if (!loading) {
//			projection = GetProjection();
//			temp = (long)projection;
//		}
//
//		if (loading) {
//			this->LockWindow(lmLock);
//		}
//
//		try
//		{
//			PX_Long( pPX, "Projection", temp, 0 );			// PROJECTION_NONE
//			projection = (tkMapProjection)temp;
//			if (loading) {
//				SetProjection(projection);
//			}
//			
//			PX_Double( pPX, "xMin", _extents.left, .3 );
//			PX_Double( pPX, "xMax", _extents.right, .3 );
//			PX_Double( pPX, "yMin", _extents.bottom, .3 );
//			PX_Double( pPX, "yMax", _extents.top, .3 );
//
//			if (loading) {
//				SetExtentsCore(_extents, false);
//			}
//		}
//		catch(...) {
//		}
//		
//		if (loading) {
//			this->LockWindow(lmUnlock);
//		}
//
//		PX_Bool( pPX, "ShowZoomBar", _zoombarVisible, TRUE );
//		PX_Bool( pPX, "GrabProjectionFromData", _grabProjectionFromData, TRUE );
//
//		temp = (long)_zoomAnimation;
//		PX_Long( pPX, "AnimationOnZooming", temp, 0 );			// csAuto
//		_zoomAnimation = (tkCustomState)temp;
//
//		temp = (long)_panningInertia;
//		PX_Long( pPX, "InertiaOnPanning", temp, 0 );			// csAuto
//		_panningInertia = (tkCustomState)temp;
//
//		PX_Bool( pPX, "ReuseTileBuffer", _reuseTileBuffer, TRUE );
//
//		temp = (long)_zoomBarVerbosity;
//		PX_Long( pPX, "ZoomBarVerbosity", temp, 1 );			// zbvFull
//		_zoomBarVerbosity = (tkZoomBarVerbosity)temp;
//
//		temp = (long)_zoomBoxStyle;
//		PX_Long( pPX, "ZoomBoxStyle", temp, 4 );			// zbsBlue
//		_zoomBoxStyle = (tkZoomBoxStyle)temp;
//
//		PX_Long( pPX, "ZoombarMinZoom", _zoomBarMinZoom, -1 );
//		PX_Long( pPX, "ZoombarMaxZoom", _zoomBarMaxZoom, -1 );
//	}
//	catch(...)
//	{
//	}
//}
#pragma endregion

#pragma region Overidden properties
// ***********************************************************************
//	   CMapView::GetControlFlags
// ***********************************************************************
// Flags to customize MFC's implementation of ActiveX controls.
// For information on using these flags, please see MFC technical note
// #nnn, "Optimizing an ActiveX Control".
DWORD CMapView::GetControlFlags()
{
	DWORD dwFlags;
	//= CWnd::GetControlFlags();

	//// The control's output is not being clipped.
	//// The control guarantees that it will not paint outside its
	//// client rectangle.
	//dwFlags &= ~clipPaintDC;

	//// The control will not be redrawn when making the transition
	//// between the active and inactivate state.
	//dwFlags |= noFlickerActivate;

	//// The control can optimize its OnDraw method, by not restoring
	//// the original GDI objects in the device context.
	//dwFlags |= canOptimizeDraw;
	return dwFlags;
}

// ***************************************************************
//	CMapView::PreCreateWindow()
// ***************************************************************
BOOL CMapView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CS_OWNDC | CS_BYTEALIGNWINDOW;
	return CWnd::PreCreateWindow(cs);
}
#pragma endregion


#pragma region GdiPlus
// ***************************************************************
//	CMapView::GdiplusStartup()
// ***************************************************************

/* Gdiplus startup/shutdown. Do it on a per control basis.
NB: this used to be in CMapViewWinGISApp::Init/ExitInstance. 
These functions are called from DllMain during control registration
and MSDN says it's not allowed to call GdiplusStartup/Shutdown from DllMain.
Indeed, on our Win2003 and XP boxes, doing so hangs regsvr32.exe.

So, I decided on "gdiplus registration per control" strategy:
1. in ctor/dtor of the control, we bump up/down gdiplus count.
2. when count goes from/to 0, we call GdiplusStartup/Shutdown.
3. we protect GdiplusStartup/Shutdown calls by a critical section,
   to avoid multithreaded surprises (albeit multiple threads are, I think, highly unlikely).*/

// IMPORTANT: the behavior is changed on 13 oct 14, startup/shutdown is moved back to
// App::InitInstaance/App::ExitInstance because of issues with tile cache, which uses
// GdiPlus::Bitmaps. It's desirable to keep it open even when the last CMapView contol
// is destructed. Solution described at http://mikevdm.com/BlogEntry/Key/GdiplusShutdown-Hangs-Mysteriously
// was implemented. Hopefully it will work.

ULONG_PTR CMapView::ms_gdiplusToken=NULL;
ULONG_PTR CMapView::ms_gdiplusBGThreadToken=NULL;
unsigned CMapView::ms_gdiplusCount=0;
::CCriticalSection CMapView::ms_gdiplusLock;
Gdiplus::GdiplusStartupOutput CMapView::ms_gdiplusStartupOutput;

void CMapView::GdiplusStartup()
{
	CSingleLock l(&ms_gdiplusLock, TRUE);
	if (ms_gdiplusCount == 0)
	{
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		gdiplusStartupInput.SuppressBackgroundThread = TRUE;
		Gdiplus::Status s = Gdiplus::GdiplusStartup(&ms_gdiplusToken, &gdiplusStartupInput, &ms_gdiplusStartupOutput);
		if (s != Gdiplus::Ok)
		{
			TRACE(_T("GdiplusStartup failed, error: %d, GetLastError = %d\n"), s, GetLastError());
			ASSERT(FALSE);
			AfxThrowResourceException();
		}
		else {
			ms_gdiplusStartupOutput.NotificationHook(&ms_gdiplusBGThreadToken);
		}
	}
	ms_gdiplusCount++;
}

// ***************************************************************
//	CMapView::GdiplusShutdown()
// ***************************************************************
void CMapView::GdiplusShutdown()
{
	CSingleLock l(&ms_gdiplusLock, TRUE);
	ms_gdiplusCount--;
	if (ms_gdiplusCount == 0)
	{
		ms_gdiplusStartupOutput.NotificationUnhook(ms_gdiplusBGThreadToken);
		Gdiplus::GdiplusShutdown(ms_gdiplusToken);
		ms_gdiplusToken = NULL;
	}
}
#pragma endregion

// ***************************************************************
//	DoUpdateTiles
// ***************************************************************
void CMapView::DoUpdateTiles(bool isSnapshot, CString key)
{
	((CTiles*)_tiles)->LoadTiles((void*)this, isSnapshot, key);		// simply move the to the screen buffer (is performed synchronously)
}

// ***************************************************************
//	ClearPanningList
// ***************************************************************
void CMapView::ClearPanningList()
{
	if (_panningInertia != csFalse)
	{
		_panningLock.Lock();
		for(int i = 0; i < _panningList.size(); i++)
		{
			delete _panningList[i];
		}
		_panningList.clear();
		_panningLock.Unlock();
	}
}

// ***************************************************************
//	GetMeasuringBase
// ***************************************************************
MeasuringBase* CMapView::GetMeasuringBase()
{
	return ((CMeasuring*)_measuring)->GetBase();
}

// ***************************************************************
//	GetShapeEditorBase
// ***************************************************************
EditorBase* CMapView::GetEditorBase()
{
	return _shapeEditor->GetActiveShape();
}

// ***************************************************************
//	GetACtiveShape
// ***************************************************************
ActiveShape* CMapView::GetActiveShape()
{
	if (m_cursorMode == cmMeasure) return GetMeasuringBase();
	return GetEditorBase();
}

// ***************************************************************
//	GetLayerVisibleAtCurrentScale
// ***************************************************************
VARIANT_BOOL CMapView::GetLayerVisibleAtCurrentScale(LONG LayerHandle)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (!IsValidLayer(LayerHandle)) return VARIANT_FALSE;
	Layer * l = _allLayers[LayerHandle];
	if (IS_VALID_PTR(l))
	{
		double scale = GetCurrentScale();
		int zoom = GetCurrentZoom();
		return l->IsVisible(scale, zoom) ? VARIANT_TRUE: VARIANT_FALSE;
	}
	return VARIANT_FALSE;
}

// ***************************************************************
//	ZoomToEditor
// ***************************************************************
void CMapView::ZoomToEditor()
{
	CShape* shp = NULL;
	_shapeEditor->get_RawData(&shp);
	if (shp) {
		CExtents* ext = NULL;
		shp->get_Extents(&ext);
		if (ext) {
			double xMin, xMax, yMin, yMax, zMin, zMax;
			ext->GetBounds(&xMin, &yMin, &zMin, &xMax, &yMax, &zMax);
			Extent temp(xMin, xMax, yMin, yMax);
			if (_extents.Intersects(temp)) return;

			CPointClass* pnt = NULL;
			ext->get_Center(&pnt);
			if (pnt) {
				double x, y;
				pnt->get_X(&x);
				pnt->get_Y(&y);
				double ratio = 2.0;   // increase the size
				double w = _extents.Width() * ratio;
				double h = _extents.Height() * ratio;
				Extent newExt(x - w/2.0, x + w/2.0, y - h/2.0, y + h / 2.0);
				SetExtentsWithPadding(newExt);
			}
		}
	}
}

// ***************************************************************
//	StartDragging
// ***************************************************************
void CMapView::StartDragging(DraggingOperation operation)
{
	_dragging.Operation = operation;
	SetCapture();
}

void CMapView::OnDraw(CDC* /*pDC*/)
{
	// TODO: 在此添加专用代码和/或调用基类
}


void CMapView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(&rect);
	OnDraw(GetDC(), rect, rect);
}
///////////////////////////////////////////////////////////////////////////
// File name handling
//
CString GetFileDirectory(const CString& path)
{
	ASSERT(path.GetLength());
	int pos = path.ReverseFind('\\');
	if (pos >= 0)
		return path.Left(pos);
	return "";
}

CString GetFileName(const CString& path)
{
	ASSERT(path.GetLength());
	int pos = path.ReverseFind('\\');
	if (pos >= 0)
		return path.Right(path.GetLength() - pos - 1);
	return "";
}

CString GetFileExt(const CString& path)
{
	ASSERT(path.GetLength());
	int pos = path.ReverseFind('.');
	if (pos >= 0)
		return path.Right(path.GetLength() - pos - 1);
	return "";
}

CString GetFileTitle(const CString& path)
{
	ASSERT(path.GetLength());
	CString strResult = GetFileName(path);
	int pos = strResult.ReverseFind('.');
	if (pos >= 0)
		return strResult.Left(pos);
	return strResult;
}

void CMapView::OnFileOpen()
{
	SetTileProvider(tkTileProvider::ProviderCustom);
	CStringArray ImgExtent;
	CStringArray ImgName;
	VARIANT_BOOL vb = VARIANT_TRUE;
	CString filter(TEXT("ESRI Shapefiles (*.shp)|*.shp|"));
	CString allFormats = TEXT("All supported formats|*.shp");
	//jpg
	ImgName.Add("Jpeg文件"); ImgExtent.Add("*.jpg");
	//BMP
	ImgName.Add("Bitmap文件"); ImgExtent.Add("*.bmp");
	//Tiff
	ImgName.Add("Tiff文件"); ImgExtent.Add("*.tif");
	//Image
	ImgName.Add("Image文件"); ImgExtent.Add("*.img");
	//Grid Text Format
	ImgName.Add("Grid文件"); ImgExtent.Add("*.asc");
	//Grid ESRI Format
	ImgName.Add("Grid文件"); ImgExtent.Add("*.adf");
	//Grid ESRI Format
	ImgName.Add("Grid文件"); ImgExtent.Add("*.bil");
	for (int i = 0; i < ImgName.GetSize(); i++)
	{
		if (ImgName[i].GetLength() == 0) continue;

		filter += ImgName[i] + TEXT("|");
		filter += ImgExtent[i] + TEXT("|");
		allFormats += TEXT(";") + ImgExtent[i];
	}
	filter = allFormats + TEXT("|") + filter + TEXT("|");
	CFileDialog dlg(TRUE, TEXT(".shp"), 0, OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, filter);
	if (dlg.DoModal() == IDOK)
	{
		POSITION pos1 = dlg.GetStartPosition();
		while (pos1)//Add MultiLayers
		{
			CString pathName = dlg.GetNextPathName(pos1);
			CString ext = GetFileExt(pathName);
			if (ext.CompareNoCase(TEXT("shp")) == 0)
			{
				CShapefile *sf = new CShapefile();
				sf->Open(pathName.AllocSysString(), NULL, &vb);
				AddSingleLayer(dynamic_cast<ILayer*>(sf), TRUE);
			}
			else if (ext.CompareNoCase(TEXT("asc")) == 0 || ext.CompareNoCase(TEXT("txt"))==0|| ext.CompareNoCase(TEXT("bil")) == 0)
			{
				CGrid *grd = new CGrid();
				CImageClass *img = NULL;
				CGridColorScheme *schem = new CGridColorScheme();
				CGridColorBreak *bk = new CGridColorBreak();
				//schem->get_Break(0, &bk);
				VARIANT vt1,vt2;
				grd->get_Minimum(&vt1);
				grd->get_Maximum(&vt2);
				bk->put_GradientModel(GradientModel::Linear);
				bk->get_LowColor(RGB(0,0,0));
				bk->put_LowValue(vt1.lVal);
				bk->put_HighColor(RGB(255,255,255));
				bk->put_HighValue(vt2.lVal);
				schem->InsertBreak(bk);
    			grd->Open(pathName.AllocSysString(), GridDataType::LongDataType, VARIANT_TRUE, GridFileType::UseExtension, NULL,&vb);
				grd->OpenAsImage(schem, tkGridProxyMode::gpmAuto, NULL, &img);
				AddSingleLayer(dynamic_cast<CImageClass*>(img), TRUE);
			}
			else if (ext.CompareNoCase(TEXT("jpg")) == 0 ||
				ext.CompareNoCase(TEXT("bmp")) == 0 ||
				ext.CompareNoCase(TEXT("tif")) == 0 ||
				ext.CompareNoCase(TEXT("img")) == 0)
			{
				CImageClass *img = new CImageClass(); 
				img->Open(pathName.AllocSysString(),ImageType::USE_FILE_EXTENSION,vb, NULL, &vb);
				AddSingleLayer(dynamic_cast<ILayer*>(img), TRUE);
			}

		}
	}
 }



//漫游
void CMapView::On32772()
{
	// TODO: 在此添加命令处理程序代码
	this->SetCursorMode(cmPan);
}

//缩放
void CMapView::On32773()
{
	// TODO: 在此添加命令处理程序代码
	//this->ZoomToWorld();
	this->ZoomToLayer(0);
}

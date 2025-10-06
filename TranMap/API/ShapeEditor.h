// ShapeEditor.h : Declaration of the CShapeEditor
#pragma once
#include "EditorBase.h"



// CShapeEditor
class  CShapeEditor 
{
public:
	CShapeEditor()
	{
		_activeShape = new EditorBase();
		_activeShape->AreaDisplayMode = admMetric;
		_lastErrorCode = tkNO_ERROR;
		_globalCallback = NULL;
		_key = SysAllocString(L"");
		_layerHandle = -1;
		_shapeIndex = -1;
		_visible = true;
		_highlightShapes = lsAllLayers;
		_snapTolerance = 10;
		_snapBehavior = lsAllLayers;
		_state = esNone;
		_mapCallback = NULL;
		_isSubjectShape = false;
		_validationMode = evCheckWithGeos;
		_redrawNeeded = false;
		_layerRedrawNeeded = false;
		_overlayType = eoAddPart;
		_behavior = ebVertexEditor;
		_startingUndoCount = -1;
	}
	~CShapeEditor()
	{
		SysFreeString(_key);
		Clear();
		delete _activeShape;
	}

public:
	void get_LastErrorCode(/*[out, retval]*/ long *pVal);
	void get_ErrorMsg(/*[in]*/ long ErrorCode, /*[out, retval]*/ BSTR *pVal);
	void get_GlobalCallback(/*[out, retval]*/ ICallback * *pVal);
	void put_GlobalCallback(/*[in]*/ ICallback * newVal);
	void get_Key(/*[out, retval]*/ BSTR *pVal);
	void put_Key(/*[in]*/ BSTR newVal);
	void Clear();
	void get_numPoints(long* retVal);
	void get_PointXY(long pointIndex, double* x, double* y, VARIANT_BOOL* retVal);
	void put_PointXY(long pointIndex, double x, double y, VARIANT_BOOL* retVal);
	void UndoPoint(VARIANT_BOOL* retVal);
	void get_SegmentLength(int segmentIndex, double* retVal);
	void get_SegmentAngle(int segmentIndex, double* retVal);
	void get_IsDigitizing(VARIANT_BOOL* retVal);
	void get_ShapeType(ShpfileType* retVal);
	void put_ShapeType(ShpfileType newVal);
	void SetShape(CShape* shp);
	void get_ValidatedShape(CShape** retVal);
	void get_LayerHandle(LONG* retVal);
	void put_LayerHandle(LONG newVal);
	void get_ShapeIndex(LONG* retVal);
	void put_ShapeIndex(LONG newVal);
	void get_Area(double* retVal);
	void get_ShapeVisible(VARIANT_BOOL* val);
	void put_ShapeVisible(VARIANT_BOOL newVal);
	void get_SelectedVertex(int* val);
	void put_SelectedVertex(int newVal);
	void get_RawData(CShape** pVal);
	void get_FillColor(OLE_COLOR* pVal);
	void put_FillColor(OLE_COLOR newVal);
	void get_FillTransparency(BYTE* pVal);
	void put_FillTransparency(BYTE newVal);
	void get_LineColor(OLE_COLOR* pVal);
	void put_LineColor(OLE_COLOR newVal);
	void get_LineWidth(FLOAT* pVal);
	void put_LineWidth(FLOAT newVal);
	void CopyOptionsFrom(CShapeDrawingOptions* options);
	void get_IsEmpty(VARIANT_BOOL* pVal);
	void get_SnapTolerance(DOUBLE* pVal);
	void put_SnapTolerance(DOUBLE newVal);
	void get_HighlightVertices(tkLayerSelection* pVal);
	void put_HighlightVertices(tkLayerSelection newVal);
	void get_SnapBehavior(tkLayerSelection* pVal);
	void put_SnapBehavior(tkLayerSelection newVal);
	void get_EditorState(tkEditorState* pVal);
	void put_EditorState(tkEditorState newVal);
	void StartEdit(LONG LayerHandle, LONG ShapeIndex, VARIANT_BOOL* retVal);
	void get_IndicesVisible(VARIANT_BOOL* pVal);
	void put_IndicesVisible(VARIANT_BOOL newVal);
	void get_AreaDisplayMode(tkAreaDisplayMode* retVal);
	void put_AreaDisplayMode(tkAreaDisplayMode newVal);
	void get_BearingType(tkBearingType* retVal);
	void put_BearingType(tkBearingType newVal);
	void get_LengthDisplayMode(tkLengthDisplayMode* pVal);
	void put_LengthDisplayMode(tkLengthDisplayMode newVal);
	void ClearSubjectShapes();
	void get_NumSubjectShapes(LONG* pVal);
	void StartUnboundShape(ShpfileType shpTYpe, VARIANT_BOOL* retVal);
	void get_VerticesVisible(VARIANT_BOOL* pVal);
	void put_VerticesVisible(VARIANT_BOOL newVal);
	void get_ValidationMode(tkEditorValidation* pVal);
	void put_ValidationMode(tkEditorValidation newVal);
	void StartOverlay(tkEditorOverlay overlayType, VARIANT_BOOL* retVal);
	void get_EditorBehavior(tkEditorBehavior* pVal);
	void put_EditorBehavior(tkEditorBehavior newVal);
	void SaveChanges(VARIANT_BOOL* retVal);
	void get_HasChanges(VARIANT_BOOL* pVal);
	void get_IsUsingEllipsoid(VARIANT_BOOL* pVal);
	void get_Length(DOUBLE* pVal);
	void get_ShowArea(VARIANT_BOOL* pVal);
	void put_ShowArea(VARIANT_BOOL newVal);
	void get_AreaPrecision(LONG* pVal);
	void put_AreaPrecision(LONG newVal);
	void get_LengthPrecision(LONG* pVal);
	void put_LengthPrecision(LONG newVal);
	void get_AngleFormat(tkAngleFormat* pVal);
	void put_AngleFormat(tkAngleFormat newVal);
	void get_AnglePrecision(LONG* pVal);
	void put_AnglePrecision(LONG newVal);

private:
	BSTR _key;
	ICallback * _globalCallback;
	VARIANT_BOOL _visible;
	tkLayerSelection _highlightShapes;
	double _snapTolerance;
	tkLayerSelection _snapBehavior;
	EditorBase* _activeShape;
	long _layerHandle;
	long _shapeIndex;
	long _lastErrorCode;
	tkEditorState _state;
	vector<CShapeEditor*> _subjects;
	bool _isSubjectShape;
	tkEditorValidation _validationMode;
	bool _redrawNeeded;
	bool _layerRedrawNeeded;
	tkEditorOverlay _overlayType;
	tkEditorBehavior _behavior;
	long _startingUndoCount;
	
public:
	IMapViewCallback* _mapCallback;

public:
	void ErrorMessage(long ErrorCode);
	void CopyData(int firstIndex, int lastIndex, CShape* target);

	void SetMapCallback(IMapViewCallback* callback) {
		_activeShape->SetMapCallback(callback, simEditing);
		_mapCallback = callback;
	}

	bool GetRedrawNeeded(RedrawTarget target) 
	{ 
		if (target == rtShapeEditor)
			return _redrawNeeded; 
		if (target == rtVolatileLayer)
			return _layerRedrawNeeded;
		return false;
	}
	void SetRedrawNeeded(RedrawTarget target) 
	{ 
		if (target == rtShapeEditor) 
			_redrawNeeded = true;
		if (target == rtVolatileLayer) 
			_layerRedrawNeeded = true; 
	}
	void ClearRedrawFlag()
	{
		_redrawNeeded = false;
		_layerRedrawNeeded = false;
	}

	EditorBase* GetActiveShape() { return _activeShape; }
	void SetIsSubject(bool value) { _isSubjectShape = value; }
	void DiscardState();
	void SaveState();
	void MoveShape(double offsetX, double offsetY);
	void MovePart(double offsetX, double offsetY);
	bool InsertVertex(double xProj, double yProj);
	bool RemoveVertex();
	bool RemovePart();
	bool CheckState();
	void Render(Gdiplus::Graphics* g, bool dynamicBuffer, DraggingOperation offsetType, int screenOffsetX, int screenOffsetY);
	void EndOverlay(CShape* overlayShape);
	CShape* GetLayerShape(long layerHandle, long shapeIndex);
	bool GetClosestPoint(double projX, double projY, double& xResult, double& yResult);
	bool HandleDelete();
	bool RemoveShape();
	int GetClosestPart(double projX, double projY, double tolerance);
	bool RestoreState(CShape* shp, long layerHandle, long shapeIndex);
	bool TryStop();
	void HandleProjPointAdd(double projX, double projY);
	bool HasSubjectShape(int LayerHandle, int ShapeIndex);
	bool ValidateWithGeos(CShape** shp);
	bool Validate(CShape** shp);
	ShpfileType GetShapeTypeForTool(tkCursorMode cursor);
	void ApplyColoringForTool(tkCursorMode mode);
	bool StartUnboundShape(tkCursorMode cursor);
	CShapeEditor* Clone();
	void ApplyOverlayColoring(tkEditorOverlay overlay);
	bool TrySaveShape(CShape* shp);
	CShape* CalculateOverlay(CShape* overlay);
	void CopyOptionsFromShapefile();
	void CancelOverlay(bool restoreSubjectShape);
	bool ClearCore(bool all);
	void DiscardChangesFromUndoList();
	
	// exposing active shape API
	bool SetSelectedVertex(int vertexIndex) { return _activeShape->SetSelectedVertex(vertexIndex); }
	bool SetSelectedPart(int partIndex) { return _activeShape->SetSelectedPart(partIndex); }
	bool HasSelectedVertex() { return _activeShape->HasSelectedVertex(); }
	bool HasSelectedPart() { return _activeShape->HasSelectedPart(); }
	int SelectPart(double xProj, double yProj) { return _activeShape->SelectPart(xProj, yProj); }
	int GetClosestVertex(double projX, double projY, double tolerance) { return _activeShape->GetClosestVertex(projX, projY, tolerance); }
	void get_ShowBearing(VARIANT_BOOL* pVal);
	void put_ShowBearing(VARIANT_BOOL newVal);
	void get_ShowLength(VARIANT_BOOL* pVal);
	void put_ShowLength(VARIANT_BOOL newVal);
	void Serialize(BSTR* retVal);
	void Deserialize(BSTR state, VARIANT_BOOL* retVal);
};

// ShapeEditor.cpp : Implementation of CShapeEditor

#include "stdafx.h"
#include "ShapeEditor.h"
#include "Shape.h"
#include "map.h"
#include "UndoList.h"
#include <set>
#include "GeosHelper.h"
#include "EditorHelper.h"
#include "ShapefileHelper.h"
#include "MeasuringHelper.h"

// *******************************************************
//		GetShape
// *******************************************************
CShape* CShapeEditor::GetLayerShape(long layerHandle, long shapeIndex)
{
	if (!CheckState()) return NULL;
	CShapefile* sf =_mapCallback->_GetShapefile(layerHandle);
	if (sf)
	{
		CShape* shp = NULL;
		sf->get_Shape(shapeIndex, &shp);
		return shp;
	}
	return NULL;
}

//***********************************************************************/
//*			CheckState()
//***********************************************************************/
bool CShapeEditor::CheckState()
{
	if (!_mapCallback) {
		ErrorMessage(tkSHAPE_EDITOR_NO_MAP);
	}
	return _mapCallback != NULL;
}

//***********************************************************************/
//*			ErrorMessage()
//***********************************************************************/
void CShapeEditor::ErrorMessage(long ErrorCode)
{
	_lastErrorCode = ErrorCode;
	CallbackHelper::ErrorMsg("ShapeEditor", _globalCallback, _key, ErrorMsg(_lastErrorCode));
}

// *****************************************************************
//	   get_ErrorMsg()
// *****************************************************************
void  CShapeEditor::get_ErrorMsg(long ErrorCode, BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	*pVal = A2BSTR(ErrorMsg(ErrorCode));
	return ;
}

// ************************************************************
//		get_LastErrorCode()
// ************************************************************
void  CShapeEditor::get_LastErrorCode(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
		*pVal = _lastErrorCode;
	_lastErrorCode = tkNO_ERROR;
	return ;
}

// ************************************************************
//		get_GlobalCallback()
// ************************************************************
void  CShapeEditor::get_GlobalCallback(ICallback **pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
		*pVal = _globalCallback;
	return ;
}

// ************************************************************
//		put_GlobalCallback()
// ************************************************************
void  CShapeEditor::put_GlobalCallback(ICallback *newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
		_globalCallback = newVal;
	return ;
}


// ************************************************************
//		get/put_Key()
// ************************************************************
void  CShapeEditor::get_Key(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
		USES_CONVERSION;
	*pVal = OLE2BSTR(_key);
	return ;
}
void  CShapeEditor::put_Key(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
		::SysFreeString(_key);
	_key = OLE2BSTR(newVal);
	return ;
}

// *******************************************************
//		ShapeType()
// *******************************************************
void  CShapeEditor::get_ShapeType(ShpfileType* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = _activeShape->GetShapeType2D();
	return ;
}

void  CShapeEditor::put_ShapeType(ShpfileType newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_activeShape->SetShapeType(newVal);
	return ;
}

// *******************************************************
//		RawData()
// *******************************************************
void  CShapeEditor::get_RawData(CShape** retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = NULL;

	ShpfileType shpType = _activeShape->GetShapeType2D();
	if (shpType == SHP_NULLSHAPE || _activeShape->GetPointCount() == 0) {
		return ;
	}

	*retVal = new CShape();
	

	VARIANT_BOOL vb;
	(*retVal)->Create(shpType, &vb);

	CopyData(0, _activeShape->GetPointCount(), *retVal);

	return ;
}

// *******************************************************
//		get_ValidatedShape()
// *******************************************************
void  CShapeEditor::get_ValidatedShape(CShape** retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = NULL;

	CShape* shp = NULL;
	get_RawData(&shp);
	
	if (Validate(&shp)) {
		*retVal = shp;
	}
	return ;
}

// *******************************************************
//		CopyData()
// *******************************************************
void CShapeEditor::CopyData(int firstIndex, int lastIndex, CShape* target )
{
	long index, partCount = 0;
	VARIANT_BOOL vb;
	for(int i = firstIndex; i < lastIndex; i++)
	{
		MeasurePoint* pnt = _activeShape->GetPoint(i);
		if (pnt) {
			target->AddPoint(pnt->Proj.x, pnt->Proj.y, &index);
			if (pnt->Part == PartBegin) {
				target->InsertPart(i, &partCount, &vb);
				partCount++;
			}
		}
	}
}

// *******************************************************
//		get_PointXY()
// *******************************************************
void  CShapeEditor::get_PointXY(long pointIndex, double* x, double* y, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (pointIndex < 0 || pointIndex >= (long)_activeShape->GetPointCount())
	{
		ErrorMsg(tkINDEX_OUT_OF_BOUNDS);
		*retVal = VARIANT_FALSE;
	}
	else
	{
		MeasurePoint* pnt = _activeShape->GetPoint(pointIndex);
		if (pnt)
		{
			*x = pnt->Proj.x;
			*y = pnt->Proj.y;
			*retVal = VARIANT_TRUE;
		}
	}
	return ;
}

// *******************************************************
//		put_PointXY()
// *******************************************************
void  CShapeEditor::put_PointXY(long pointIndex, double x, double y, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (pointIndex < 0 || pointIndex >= (long)_activeShape->GetPointCount())
	{
		ErrorMsg(tkINDEX_OUT_OF_BOUNDS);
		*retVal = VARIANT_FALSE;
	}
	else
	{
		_activeShape->UpdatePoint(pointIndex, x, y);
		*retVal = VARIANT_TRUE;
	}
	return ;
}

// *******************************************************
//		get_PointCount()
// *******************************************************
void  CShapeEditor::get_numPoints(long* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = _activeShape->GetPointCount();
	return ;
}

// *******************************************************
//		ClearCore()
// *******************************************************
bool CShapeEditor::ClearCore(bool all)
{
	if (!CheckState()) return true;

	if (_state == esOverlay)
	{
		CancelOverlay(true);
		if (!all) return false;    // clear overlay only; one more call is needed to finish it
	}

	if (_state == esEdit) {

		CShapefile* sf =_mapCallback->_GetShapefile(_layerHandle);

		if (sf) {
			sf->put_ShapeIsHidden(_shapeIndex, VARIANT_FALSE);
		}

		if (_startingUndoCount != -1) {
			DiscardChangesFromUndoList();
		}
	}

	_activeShape->Clear();

	_startingUndoCount = -1;
	_shapeIndex = -1;
	_layerHandle = -1;
	_state = esNone;
	return true;
}

// *******************************************************
//		DiscardChanges()
// *******************************************************
void CShapeEditor::DiscardChangesFromUndoList()
{
	if (_startingUndoCount == -1) return;
	
	CUndoList* undoList = _mapCallback->_GetUndoList();
	if (undoList)
	{
		// as we discard the changes, clear them from undo list;
		// it's also possible to actually call Undo rather than DiscardState
		// but there is no point in doing so, as we aren't going to substitute
		// the underlying shape
		long length;
		undoList->get_UndoCount(&length);
		long undoCount = length - _startingUndoCount;
		for (long i = 0; i < undoCount; i++)
			((CUndoList*)undoList)->DiscardOne();
		if (undoCount > 0) {
			_mapCallback->_FireUndoListChanged();
		}
	}
}

// *******************************************************
//		Clear()
// *******************************************************
void  CShapeEditor::Clear()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ClearCore(true);
	return ;
}

// *******************************************************
//		StartEdit
// *******************************************************
void  CShapeEditor::StartEdit(LONG LayerHandle, LONG ShapeIndex, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = VARIANT_FALSE;
	if (!CheckState()) return ;

	if (LayerHandle == -1 || ShapeIndex == -1) {
		ErrorMessage(tkINDEX_OUT_OF_BOUNDS);
		return ;
	}

	CShapefile* sf = _mapCallback->_GetShapefile(LayerHandle);
	if (!sf) {
		ErrorMessage(tkINVALID_LAYER_HANDLE);
		return ;
	}

	if (!ShapefileHelper::InteractiveEditing(sf))
	{
		ErrorMessage(tkNO_INTERACTIVE_EDITING);
		return ;
	}

	Clear();

	CUndoList* list = _mapCallback->_GetUndoList();
	if (list) {
		list->get_UndoCount(&_startingUndoCount);
	}
	
	CShape* shp = NULL;
	sf->get_Shape(ShapeIndex, &shp);
	if (shp)
	{
		put_EditorState(esEdit);
		SetShape(shp);
		_layerHandle = LayerHandle;
		_shapeIndex = ShapeIndex;
		sf->put_ShapeIsHidden(ShapeIndex, VARIANT_TRUE);
		EditorHelper::CopyOptionsFrom(this, sf);
		*retVal = VARIANT_TRUE;
	}
	return ;
}

// ***************************************************************
//		SetShape()
// ***************************************************************
void  CShapeEditor::SetShape( CShape* shp )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (!shp) {
		ErrorMessage(tkINVALID_PARAMETER_VALUE);
		return ;
	}

	_activeShape->Clear();

	ShpfileType shpType;
	shp->get_ShapeType(&shpType);
	_activeShape->SetShapeType(shpType);
	_activeShape->SetCreationMode(false);

	long numPoints, numParts;
	shp->get_NumParts(&numParts);
	std::set<long> parts, endParts;
	for (long j = 0; j < numParts; j++) {
		long part;
		shp->get_Part(j, &part);
		parts.insert(part);
		shp->get_EndOfPart(j, &part);
		endParts.insert(part);
	}

	VARIANT_BOOL vb;
	double x, y;
	shp->get_NumPoints(&numPoints);
	
	for(long i = 0; i < numPoints; i++) 
	{
		PointPart part = PartNone;
		if (parts.find(i) != parts.end()) part = PartBegin;
		if (endParts.find(i) != endParts.end()) part = PartEnd;

		shp->get_XY(i, &x, &y, &vb);
		_activeShape->AddPoint(x, y, -1, -1, part);
	}
	return ;
}

// *******************************************************
//		SegmentLength()
// *******************************************************
void  CShapeEditor::get_SegmentLength(int segmentIndex, double* retVal) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	long errorCode = tkNO_ERROR;
	*retVal = _activeShape->GetSegmentLength(segmentIndex, errorCode);
	return ;
}

// *******************************************************
//		SegmentAngle()
// *******************************************************
void  CShapeEditor::get_SegmentAngle(int segmentIndex, double* retVal) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	long errorCode = tkNO_ERROR;
	*retVal = _activeShape->GetSegmentAngle(segmentIndex, errorCode);
	return ;
}

// *******************************************************
//		AreaDisplayMode()
// *******************************************************
void  CShapeEditor::get_AreaDisplayMode(tkAreaDisplayMode* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = _activeShape->AreaDisplayMode;
	return ;
}

void  CShapeEditor::put_AreaDisplayMode(tkAreaDisplayMode newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_activeShape->AreaDisplayMode = newVal;
	return ;
}

// *******************************************************
//		AngleDisplayMode()
// *******************************************************
void  CShapeEditor::get_BearingType(tkBearingType* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = _activeShape->BearingType;
	return ;
}

void  CShapeEditor::put_BearingType(tkBearingType newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_activeShape->BearingType = newVal;
	return ;
}

// *******************************************************
//		CreationMode()
// *******************************************************
void  CShapeEditor::get_IsDigitizing(VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = _activeShape->GetCreationMode() ? VARIANT_TRUE : VARIANT_FALSE;
	return ;
}

// *******************************************************
//		get_Area()
// *******************************************************
void  CShapeEditor::get_Area(double* retVal) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = _activeShape->GetArea(false, 0.0, 0.0);
	return ;
}

// *******************************************************
//		LayerHandle()
// *******************************************************
void  CShapeEditor::get_LayerHandle(LONG* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = _layerHandle;
	return ;
}

void  CShapeEditor::put_LayerHandle(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_layerHandle = newVal;
	return ;
}

// *******************************************************
//		ShapeIndex()
// *******************************************************
void  CShapeEditor::get_ShapeIndex(LONG* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = _shapeIndex;
	return ;
}

void  CShapeEditor::put_ShapeIndex(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_shapeIndex = newVal;
	return ;
}

// *******************************************************
//		LabelsOnly()
// *******************************************************
void  CShapeEditor::get_ShapeVisible(VARIANT_BOOL* val)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*val = _activeShape->_drawLabelsOnly;
	return ;
}
void  CShapeEditor::put_ShapeVisible(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_activeShape->_drawLabelsOnly = newVal ? true : false;
	return ;
}

// *******************************************************
//		SelectedVertex()
// *******************************************************
void  CShapeEditor::get_SelectedVertex(int* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = _activeShape->_selectedVertex;
	return ;
}

void  CShapeEditor::put_SelectedVertex(int newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_activeShape->SetSelectedVertex(newVal);
	return ;
}

// *******************************************************
//		FillColor()
// *******************************************************
void  CShapeEditor::get_FillColor(OLE_COLOR* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _activeShape->FillColor;
	return ;
}
void  CShapeEditor::put_FillColor(OLE_COLOR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_activeShape->FillColor = newVal;
	return ;
}

// *******************************************************
//		FillTransparency()
// *******************************************************
void  CShapeEditor::get_FillTransparency(BYTE* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _activeShape->FillTransparency;
	return ;
}
void  CShapeEditor::put_FillTransparency(BYTE newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_activeShape->FillTransparency = newVal;
	return ;
}

// *******************************************************
//		LineColor()
// *******************************************************
void  CShapeEditor::get_LineColor(OLE_COLOR* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _activeShape->LineColor;
	return ;
}
void  CShapeEditor::put_LineColor(OLE_COLOR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_activeShape->LineColor = newVal;
	return ;
}

// *******************************************************
//		LineWidth()
// *******************************************************
void  CShapeEditor::get_LineWidth(FLOAT* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _activeShape->LineWidth;
	return ;
}
void  CShapeEditor::put_LineWidth(FLOAT newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_activeShape->LineWidth = newVal;
	return ;
}

// *******************************************************
//		MoveShape()
// *******************************************************
void CShapeEditor::MoveShape(double offsetX, double offsetY)
{
	SaveState();
	_activeShape->Move(offsetX, offsetY);
}

// *******************************************************
//		MovePart()
// *******************************************************
void CShapeEditor::MovePart(double offsetX, double offsetY)
{
	SaveState();
	_activeShape->MovePart(offsetX, offsetY);
}

// *******************************************************
//		InsertVertex()
// *******************************************************
bool CShapeEditor::InsertVertex(double xProj, double yProj)
{
	SaveState();
	if (_activeShape->TryInsertVertex(xProj, yProj)) return true;
	DiscardState();
	return false;
}

// *******************************************************
//		RemoveVertex()
// *******************************************************
bool CShapeEditor::RemoveVertex()
{
	SaveState();
	if (_activeShape->RemoveSelectedVertex()) return true;
	DiscardState();
	return false;
}

// *******************************************************
//		RemovePart()
// *******************************************************
bool CShapeEditor::RemovePart()
{
	SaveState();
	if (_activeShape->RemovePart()) return true;
	DiscardState();
	return false;
}

// *******************************************************
//		SaveState()
// *******************************************************
void CShapeEditor::SaveState()
{
	CUndoList* undoList = _mapCallback->_GetUndoList();
	if (undoList) 
	{
		VARIANT_BOOL vb;
		undoList->Add(uoEditShape, _layerHandle, _shapeIndex, &vb);
	}
}

// *******************************************************
//		DiscardState()
// *******************************************************
void CShapeEditor::DiscardState()
{
	CUndoList* undoList = _mapCallback->_GetUndoList();
	if (undoList)
	{
		((CUndoList*)undoList)->DiscardOne();
	}
}

// *******************************************************
//		UndoPoint()
// *******************************************************
void  CShapeEditor::UndoPoint(VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = VARIANT_FALSE;

	VARIANT_BOOL digitizing;
	get_IsDigitizing(&digitizing);
	
	if (digitizing) {
		*retVal = _activeShape->UndoPoint() ? VARIANT_TRUE : VARIANT_FALSE;
	}
	return ;
}

// *******************************************************
//		get_IsEmpty()
// *******************************************************
void  CShapeEditor::get_IsEmpty(VARIANT_BOOL* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _activeShape->IsEmpty() && _subjects.size() == 0 ? VARIANT_TRUE : VARIANT_FALSE;
	return ;
}

// *******************************************************
//		SnapTolerance()
// *******************************************************
void  CShapeEditor::get_SnapTolerance(DOUBLE* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _snapTolerance;
	return ;
}
void  CShapeEditor::put_SnapTolerance(DOUBLE newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (newVal >= 0.0 && newVal <= 100.0) {
		_snapTolerance = newVal;
	}
	else {
		ErrorMessage(tkINVALID_PARAMETER_VALUE);
	}
	return ;
}

// *******************************************************
//		HighlightShapes()
// *******************************************************
void  CShapeEditor::get_HighlightVertices(tkLayerSelection* pVal)
{
	*pVal = _highlightShapes;
	return ;
}
void  CShapeEditor::put_HighlightVertices(tkLayerSelection newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_highlightShapes = newVal;
	return ;
}

// *******************************************************
//		get_SnapBehavior()
// *******************************************************
void  CShapeEditor::get_SnapBehavior(tkLayerSelection* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _snapBehavior;
	return ;
}
void  CShapeEditor::put_SnapBehavior(tkLayerSelection newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_snapBehavior = newVal;
	return ;
}

// *******************************************************
//		EditorState
// *******************************************************
void  CShapeEditor::get_EditorState(tkEditorState* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _state;
	return ;
}
void  CShapeEditor::put_EditorState(tkEditorState newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_activeShape->SetCreationMode(newVal == esDigitize || newVal == esDigitizeUnbound || newVal == esOverlay);
	_activeShape->OverlayTool = newVal == esOverlay || newVal == esDigitizeUnbound;
	_state = newVal;
	return ;
}

// *******************************************************
//		Render
// *******************************************************
void CShapeEditor::Render(Gdiplus::Graphics* g, bool dynamicBuffer, DraggingOperation offsetType, int screenOffsetX, int screenOffsetY)
{
	for (size_t i = 0; i < _subjects.size(); i++) {
		EditorBase* base = _subjects[i]->GetActiveShape();
		base->DrawData(g, dynamicBuffer, DragNone);
	}
	_activeShape->DrawData(g, dynamicBuffer, offsetType, screenOffsetX, screenOffsetY);
}

// *******************************************************
//		PointLabelsVisible
// *******************************************************
void  CShapeEditor::get_IndicesVisible(VARIANT_BOOL* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _activeShape->PointLabelsVisible ? VARIANT_TRUE: VARIANT_FALSE;
	return ;
}
void  CShapeEditor::put_IndicesVisible(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_activeShape->PointLabelsVisible = newVal ? true: false;
	return ;
}

// *******************************************************
//		LengthDisplayMode
// *******************************************************
void  CShapeEditor::get_LengthDisplayMode(tkLengthDisplayMode* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _activeShape->LengthUnits;
	return ;
}
void  CShapeEditor::put_LengthDisplayMode(tkLengthDisplayMode newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_activeShape->LengthUnits = newVal;
	return ;
}

// *******************************************************
//		ClearSubjectShapes
// *******************************************************
void  CShapeEditor::ClearSubjectShapes()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	for (size_t i = 0; i < _subjects.size(); i++) {
		_subjects[i]->Clear();
		delete _subjects[i];
	}
	_subjects.clear();
	return ;
}

// *******************************************************
//		NumSubjectShapes
// *******************************************************
void  CShapeEditor::get_NumSubjectShapes(LONG* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = (LONG)_subjects.size();
	return ;
}

// *******************************************************
//		StartUnboundShape
// *******************************************************
// for built-in cursors
bool CShapeEditor::StartUnboundShape(tkCursorMode cursor)
{
	ShpfileType	shpType = GetShapeTypeForTool(cursor);
	if (shpType == SHP_NULLSHAPE)
	{
		ErrorMessage(tkINVALID_PARAMETER_VALUE);
		return false;
	}
	put_ShapeType(shpType);
	put_EditorState(esDigitizeUnbound);
	ApplyColoringForTool(cursor);
	return true;
}

// *******************************************************
//		StartUnboundShape
// *******************************************************
void  CShapeEditor::StartUnboundShape(ShpfileType shpType, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = VARIANT_FALSE;

	tkCursorMode cursor = _mapCallback->_GetCursorMode();
	if (cursor != cmAddShape) {
		ErrorMessage(tkADD_SHAPE_MODE_ONLY);
		return ;
	}

	if (shpType == SHP_NULLSHAPE)
	{
		ErrorMessage(tkINVALID_PARAMETER_VALUE);
		return ;
	}
	
	put_ShapeType(shpType);
	put_EditorState(esDigitizeUnbound);

	return ;
}

// *******************************************************
//		VerticesVisible
// *******************************************************
void  CShapeEditor::get_VerticesVisible(VARIANT_BOOL* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _activeShape->PointsVisible ? VARIANT_TRUE : VARIANT_FALSE;
	return ;
}
void  CShapeEditor::put_VerticesVisible(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_activeShape->PointsVisible = newVal ? true: false;
	return ;
}

// *******************************************************
//		GetClosestPoint
// *******************************************************
bool CShapeEditor::GetClosestPoint(double projX, double projY, double& xResult, double& yResult)
{
	ShpfileType shpType = Utility::ShapeTypeConvert2D(_activeShape->GetShapeType2D());

	if (shpType == SHP_POINT || shpType == SHP_MULTIPOINT)
		return false;				// TODO: multi points should be supported

	CShape* shp = NULL;
	get_RawData(&shp);
	if (shp)
	{
		// we need ClosestPoints method to return points on contour, and not inner points		
		if (shpType == SHP_POLYGON)
			shp->put_ShapeType(SHP_POLYLINE);

		// creating temp shape
		VARIANT_BOOL vb;
		long pointIndex = 0;
		CShape* shp2 = new CShape();
		shp2->Create(SHP_POINT, &vb);
		shp2->AddPoint(projX, projY, &pointIndex);

		CShape* result = NULL;
		shp->ClosestPoints(shp2, &result);
		if (result)
		{
			result->get_XY(0, &xResult, &yResult, &vb);		// 0 = point lying on the line
			return vb ? true : false;
		}
	}
	return false;
}

// *******************************************************
//		GetClosestPart
// *******************************************************
int CShapeEditor::GetClosestPart(double projX, double projY, double tolerance)
{
	double x, y;
	if (GetClosestPoint(projX, projY, x, y)) {
		double dist = sqrt(pow(x - projX, 2.0) + pow(y - projY, 2.0));
		if (dist < tolerance) {
			return _activeShape->SelectPart(x, y);
		}
	}
	return -1;
}

// *******************************************************
//		GetClosestPoint
// *******************************************************
bool CShapeEditor::HandleDelete()
{
	if (!CheckState()) return false;

	tkDeleteTarget target = _activeShape->GetDeleteTarget();

	if (target == dtNone) return false;

	tkMwBoolean cancel = blnFalse;
	_mapCallback->_FireBeforeDeleteShape(target, &cancel);
	if (cancel == blnTrue) return false;

	switch (target){
		case  dtShape:
			if (RemoveShape())	{
				Clear();
				return true;
			}
		case dtPart:
			return RemovePart();
		case dtVertex:
			return RemoveVertex();
	}
	return false;
}

// ************************************************************
//		RemoveShape
// ************************************************************
bool CShapeEditor::RemoveShape()
{
	if (!CheckState()) return false;
	CShapefile* sf = _mapCallback->_GetShapefile(_layerHandle);
	if (sf) 
	{
		tkMwBoolean cancel = blnFalse;
		if (cancel == blnTrue) return false;

		VARIANT_BOOL vb;
		CUndoList* undoList = _mapCallback->_GetUndoList();
		if (undoList != NULL) 
		{
			undoList->Add(uoRemoveShape, _layerHandle, _shapeIndex, &vb);
			if (vb) 
			{
				sf->EditDeleteShape(_shapeIndex, &vb);
				_startingUndoCount = -1;
				_mapCallback->_FireAfterShapeEdit(uoRemoveShape, _layerHandle, _shapeIndex);
				return true;
			}
		}
	}
	return false;
}

// ************************************************************
//		Validate
// ************************************************************
bool CShapeEditor::Validate(CShape** shp)
{
	if (!(*shp)) return false;

	ShpfileType shpType;
	(*shp)->get_ShapeType(&shpType);
	
	long numPoints;
	(*shp)->get_NumPoints(&numPoints);

	// close the poly automatically
	if (shpType == SHP_POLYGON)
		((CShape*)(*shp))->FixupShapeCore(ShapeValidityCheck::FirstAndLastPointOfPartMatch);

	VARIANT_BOOL vb;
	ShapeValidityCheck validityCheck;
	CString errMsg;
	if (!((CShape*)(*shp))->ValidateBasics(validityCheck, errMsg))
	{
		if (validityCheck == DirectionOfPolyRings) 
		{
			(*shp)->ReversePointsOrder(0, &vb);
		}
	}

	if (!((CShape*)(*shp))->ValidateBasics(validityCheck, errMsg)) {
		_mapCallback->_FireShapeValidationFailed(errMsg);
		return false;
	}

	return ValidateWithGeos(shp);
}

// ************************************************************
//		ValidateWithGeos
// ************************************************************
bool CShapeEditor::ValidateWithGeos(CShape** shp)
{
	VARIANT_BOOL valid = VARIANT_TRUE;

	ShpfileType shpType;
	(*shp)->get_ShapeType(&shpType);
	Utility::ShapeTypeConvert2D(shpType);

	bool skipGeosCheck = false;
	if (shpType == SHP_POINT || shpType == SHP_MULTIPOINT || shpType == SHP_POLYLINE)
		skipGeosCheck = true;     // there is hardly anything else to check for those

	if ((_validationMode == evCheckWithGeos || _validationMode == evFixWithGeos) && !skipGeosCheck)
	{
		(*shp)->get_IsValid(&valid);

		if (!valid && _validationMode == evFixWithGeos)
		{
			CShape* shpNew = NULL;
			(*shp)->FixUp(&shpNew);
			if (shpNew) {
				(*shp) = NULL;				// TODO: will it release the shape?
				(*shp) = shpNew;
				valid = VARIANT_TRUE;
			}
		}

		// report results back to user
		CComBSTR reason;
		if (!valid) {
			(*shp)->get_IsValidReason(&reason);
			USES_CONVERSION;
			_mapCallback->_FireShapeValidationFailed(OLE2A(reason));
		}
	}
	return valid ? true : false;
}

// ************************************************************
//		TryStopDigitizing
// ************************************************************
bool CShapeEditor::TryStop()
{
	if (!CheckState()) return false;
	
	CShape* shp = NULL;
	get_ValidatedShape(&shp);

	switch (_state)
	{
		case esDigitizeUnbound:
			_mapCallback->_UnboundShapeFinished(shp);
			SetRedrawNeeded(rtShapeEditor);
			break;
		case esOverlay:
			EndOverlay(shp);
			SetRedrawNeeded(rtShapeEditor);
			break;
		case esDigitize:
		case esEdit:
			VARIANT_BOOL isEmpty;
			get_IsEmpty(&isEmpty);
			if (isEmpty) return true;
			if (!shp) return false;
			bool result = TrySaveShape(shp);
			SetRedrawNeeded(rtVolatileLayer);
			break;
	}
	return true;
}

// ************************************************************
//		TrySaveShape
// ************************************************************
bool CShapeEditor::TrySaveShape(CShape* shp)
{
	CShapefile* sf = _mapCallback->_GetShapefile(_layerHandle);
	if (!sf) {
		ErrorMessage(tkINVALID_PARAMETER_VALUE);
		return false;
	}

	// 3) custom validation
	tkMwBoolean cancel = blnFalse;
	//_mapCallback->_FireValidateShape(_layerHandle, shp, &cancel);
	if (cancel == blnTrue) {
		return false;
	}

	VARIANT_BOOL vb;
	int shapeIndex = _shapeIndex;
	int layerHandle = _layerHandle;
	bool newShape = _shapeIndex == -1;

	if (newShape)
	{
		long numShapes = ShapefileHelper::GetNumShapes(sf);
		CUndoList* undoList = _mapCallback->_GetUndoList();
		undoList->Add(uoAddShape, (long)_layerHandle, (long)numShapes, &vb);
		sf->EditInsertShape(shp, &numShapes, &vb);
		shapeIndex = numShapes;
	}
	else 
	{
		sf->EditUpdateShape(shapeIndex, shp, &vb);
	}
	_startingUndoCount = -1;	// don't discard states; operation succeeded

	Clear();

	_mapCallback->_Redraw(tkRedrawType::RedrawAll, false, true);

	// let the user set new attributes
	_mapCallback->_FireAfterShapeEdit(newShape ? uoAddShape : uoEditShape, layerHandle, shapeIndex);

	return true;
}

// ************************************************************
//		RestoreState
// ************************************************************
bool CShapeEditor::RestoreState(CShape* shp, long layerHandle, long shapeIndex)
{
	bool newShape = layerHandle != _layerHandle || shapeIndex != _shapeIndex;
	bool hasShape = _layerHandle != -1;

	if (hasShape && newShape) {
		if (!TryStop())
			return false;
	}
	
	if (newShape) {
		Clear();
		VARIANT_BOOL vb;
		StartEdit(layerHandle, shapeIndex, &vb);
		if (!vb) return false;
	}
	
	SetShape(shp);
	_mapCallback->_SetMapCursor(cmEditShape, false);
	return true;
}

// ***************************************************************
//		HandleProjPointAdd()
// ***************************************************************
void CShapeEditor::HandleProjPointAdd(double projX, double projY)
{
	double pixelX, pixelY;
	_mapCallback->_ProjectionToPixel(projX, projY, &pixelX, &pixelY);
	_activeShape->AddPoint(projX, projY, pixelX, pixelY);
}

// ***************************************************************
//		HasSubjectShape()
// ***************************************************************
bool CShapeEditor::HasSubjectShape(int LayerHandle, int ShapeIndex)
{
	for (size_t i = 0; i < _subjects.size(); i++) 
	{
		LONG handle, index;
		_subjects[i]->get_LayerHandle(&handle);
		_subjects[i]->get_ShapeIndex(&index);
		if (LayerHandle == handle && ShapeIndex == index)
			return true;
	}
	return false;
}

// ***************************************************************
//		ValidationMode()
// ***************************************************************
void  CShapeEditor::get_ValidationMode(tkEditorValidation* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _validationMode;
	return ;
}
void  CShapeEditor::put_ValidationMode(tkEditorValidation newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_validationMode = newVal;
	return ;
}

// ***************************************************************
//		GetOverlayTypeForSubjectOperation()
// ***************************************************************
ShpfileType CShapeEditor::GetShapeTypeForTool(tkCursorMode cursor)
{
	switch (cursor)
	{
		case cmSelectByPolygon:
		case cmClipByPolygon:
		case cmSplitByPolygon:
		case cmEraseByPolygon:
			return SHP_POLYGON;
		case cmSplitByPolyline:
			return SHP_POLYLINE;
	}
	return SHP_NULLSHAPE;
}

// ***************************************************************
//		ApplyColoringForTool()
// ***************************************************************
#include "Utils.h"
void CShapeEditor::ApplyColoringForTool(tkCursorMode mode)
{
	OLE_COLOR color;
	GetUtils()->ColorByName(LightSlateGray, &color);
	put_LineColor(color);
	put_LineWidth(1.0f);
	switch (mode)
	{
		case cmClipByPolygon:
			GetUtils()->ColorByName(Green, &color);
			put_FillColor(color);
			break;
		case cmEraseByPolygon:
			GetUtils()->ColorByName(Red, &color);
			put_FillColor(color);
			break;		
		case cmSplitByPolyline:
			put_LineColor(RGB(255, 0, 0));
			break;
		case cmSelectByPolygon:
			GetUtils()->ColorByName(Yellow, &color);
			put_FillColor(color);
			put_LineColor(color);
			break;
		case cmSplitByPolygon:
			put_FillColor(RGB(127, 127, 127));
			put_LineColor(RGB(255, 0, 0));
			break;
	}
}

// ***************************************************************
//		ApplyOverlayColoring()
// ***************************************************************
void CShapeEditor::ApplyOverlayColoring(tkEditorOverlay overlay)
{
	_overlayType =  overlay;
	OLE_COLOR color;
	switch (overlay)
	{
		case eoAddPart:
			GetUtils()->ColorByName(Green, &color);
			put_FillColor(color);
			break;
		case eoRemovePart:
			GetUtils()->ColorByName(Red, &color);
			put_FillColor(color);
			break;
	}
}

// ***************************************************************
//		Clone()
// ***************************************************************
CShapeEditor* CShapeEditor::Clone()
{
	CShapeEditor* editor = new CShapeEditor();
	editor->SetIsSubject(true);
	editor->SetMapCallback(_mapCallback);

	CShapefile* sf =_mapCallback->_GetShapefile(_layerHandle);
	if (sf) {
		EditorHelper::CopyOptionsFrom(editor, sf);
	}

	CShape* shp = NULL;
	get_RawData(&shp);
	if (shp) {
		editor->SetShape(shp);
	}
	return editor;
}

// ***************************************************************
//		StartOverlay()
// ***************************************************************
void  CShapeEditor::StartOverlay(tkEditorOverlay overlayType, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = VARIANT_FALSE;
	
	if (_activeShape->IsEmpty())
	{
		ErrorMessage(tkEDITOR_OVERLAY_NO_SUBJECT);
		return ;
	}

	if (_subjects.size() > 0)
	{
		ErrorMessage(tkEDITOR_OVERLAY_ALREADY_STARTED);
		return ;
	}
	
	CShapeEditor* editor = Clone();
	_subjects.push_back(editor);
	ShpfileType shpType = overlayType == eoRemovePart ? SHP_POLYGON: _activeShape->GetShapeType2D();

	_activeShape->Clear();
	put_ShapeType(shpType);
	put_EditorState(esOverlay);
	ApplyOverlayColoring(overlayType);

	_mapCallback->_SetMapCursor(cmAddShape, false);

	return ;
}

// *******************************************************
//		CalculateOverlay
// *******************************************************
CShape* CShapeEditor::CalculateOverlay(CShape* overlay)
{
	if (!overlay) return NULL;

	if (_subjects.size() != 1) {
		return NULL;
	}

	CShape* subject = NULL;
	_subjects[0]->get_RawData(&subject);

	if (!subject || !overlay) {
		return NULL;
	}

	CShape* result = NULL;
	switch (_overlayType)
	{
		case eoAddPart:
			subject->Clip(overlay, tkClipOperation::clUnion, &result);
			break;
		case eoRemovePart:
			subject->Clip(overlay, tkClipOperation::clDifference, &result);
			break;
	}
	return result;
}

// *******************************************************
//		EndOverlay
// *******************************************************
void CShapeEditor::EndOverlay(CShape* overlay)
{
	CShape* result = CalculateOverlay(overlay);

	CancelOverlay(false);

	if (result)
	{
		VARIANT_BOOL vb;
		CUndoList* undoList = _mapCallback->_GetUndoList();
		undoList->Add(uoEditShape, _layerHandle, _shapeIndex, &vb);
		SetShape(result);
	}
}

// ***************************************************************
//		CancelOverlay()
// ***************************************************************
void CShapeEditor::CancelOverlay(bool restoreSubjectShape)
{
	if (_state == esOverlay)
	{
		if (_subjects.size() > 0)
		{
			CShape* shp = NULL;
			_subjects[0]->get_RawData(&shp);
			SetShape(shp);
		}
		ClearSubjectShapes();
		put_EditorState(esEdit);
		CopyOptionsFromShapefile();
		_mapCallback->_SetMapCursor(cmEditShape, false);
	}
}

// ***************************************************************
//		CopyOptionsFromShapefile()
// ***************************************************************
void CShapeEditor::CopyOptionsFromShapefile()
{
	CShapefile* sf = _mapCallback->_GetShapefile(_layerHandle);
	EditorHelper::CopyOptionsFrom(this, sf);
}

// ***************************************************************
//		CopyOptionsFrom()
// ***************************************************************
void  CShapeEditor::CopyOptionsFrom(CShapeDrawingOptions* options)
{
	EditorHelper::CopyOptionsFrom(this, options);
	return ;
}

// ***************************************************************
//		EditorBehavior()
// ***************************************************************
void  CShapeEditor::get_EditorBehavior(tkEditorBehavior* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _behavior;
	return ;
}
void  CShapeEditor::put_EditorBehavior(tkEditorBehavior newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_behavior = newVal;
	switch (newVal)
	{
		case ebVertexEditor:
			SetSelectedPart(-1);
			_redrawNeeded = true;
			break;
		case ebPartEditor:
			SetSelectedVertex(-1);
			_redrawNeeded = true;
			break;
	}
	return ;
}

// ***************************************************************
//		SaveChanges()
// ***************************************************************
void  CShapeEditor::SaveChanges(VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = VARIANT_FALSE;
	if (_state == esOverlay)
	{
		CancelOverlay(true);    // discard any overlay
	}
	bool result = TryStop();
	*retVal = result ? VARIANT_TRUE : VARIANT_FALSE;
	return ;
}

// ***************************************************************
//		HasChanges()
// ***************************************************************
void  CShapeEditor::get_HasChanges(VARIANT_BOOL* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = VARIANT_FALSE;
	if (!CheckState()) return ;
	
	switch (_state)
	{
		case esNone:
			*pVal = VARIANT_FALSE;
			break;
		case esDigitizeUnbound:
		case esDigitize:
			*pVal = EditorHelper::IsEmpty(this) ? VARIANT_FALSE : VARIANT_TRUE;
			break;
		case esEdit:
		case esOverlay:
			CUndoList* list = _mapCallback->_GetUndoList();
			if (list) {
				long undoCount;
				list->get_UndoCount(&undoCount);
				*pVal = undoCount > _startingUndoCount ? VARIANT_TRUE : VARIANT_FALSE;
			}
			break;
	}
	return ;
}

// ***************************************************************
//		get_IsUsingEllipsoid()
// ***************************************************************
void  CShapeEditor::get_IsUsingEllipsoid(VARIANT_BOOL* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _activeShape->GetTransformationMode() != tmNotDefined;
	return ;
}

// ***************************************************************
//		get_Length()
// ***************************************************************
void  CShapeEditor::get_Length(DOUBLE* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _activeShape->GetDistance();
	return ;
}

// ***************************************************************
//		AreaVisible()
// ***************************************************************
void  CShapeEditor::get_ShowArea(VARIANT_BOOL* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _activeShape->ShowArea;
	return ;
}

void  CShapeEditor::put_ShowArea(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_activeShape->ShowArea = newVal ? true : false;
	return ;
}

// ***************************************************************
//		AreaPrecision()
// ***************************************************************
void  CShapeEditor::get_AreaPrecision(LONG* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _activeShape->AreaPrecision;
	return ;
}

void  CShapeEditor::put_AreaPrecision(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_activeShape->AreaPrecision = newVal;
	return ;
}

// ***************************************************************
//		LengthPrecision()
// ***************************************************************
void  CShapeEditor::get_LengthPrecision(LONG* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _activeShape->LengthPrecision;
	return ;
}

void  CShapeEditor::put_LengthPrecision(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_activeShape->LengthPrecision = newVal;
	return ;
}

// ***************************************************************
//		AngleFormat()
// ***************************************************************
void  CShapeEditor::get_AngleFormat(tkAngleFormat* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _activeShape->AngleFormat;
	return ;
}

void  CShapeEditor::put_AngleFormat(tkAngleFormat newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_activeShape->AngleFormat = newVal;
	return ;
}

// ***************************************************************
//		AnglePrecision()
// ***************************************************************
void  CShapeEditor::get_AnglePrecision(LONG* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _activeShape->AnglePrecision;
	return ;
}

void  CShapeEditor::put_AnglePrecision(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_activeShape->AnglePrecision = newVal;
	return ;
}

// ***************************************************************
//		ShowBearing()
// ***************************************************************
void  CShapeEditor::get_ShowBearing(VARIANT_BOOL* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _activeShape->ShowBearing;
	return ;
}

void  CShapeEditor::put_ShowBearing(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_activeShape->ShowBearing = newVal ? true : false;
	return ;
}

// ***************************************************************
//		ShowLength()
// ***************************************************************
void  CShapeEditor::get_ShowLength(VARIANT_BOOL* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _activeShape->ShowLength;
	return ;
}

void  CShapeEditor::put_ShowLength(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_activeShape->ShowLength = newVal ? true : false;
	return ;
}

// *******************************************************
//		Serialize()
// *******************************************************
void  CShapeEditor::Serialize(BSTR* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CPLXMLNode* node = MeasuringHelper::Serialize(_activeShape, "ShapeEditorClass");
	Utility::SerializeAndDestroyXmlTree(node, retVal);

	return ;
}

// *******************************************************
//		Deserialize()
// *******************************************************
void  CShapeEditor::Deserialize(BSTR state, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	USES_CONVERSION;
	*retVal = VARIANT_FALSE;

	CString s = OLE2CA(state);
	CPLXMLNode* node = CPLParseXMLString(s.GetString());
	if (node)
	{
		CPLXMLNode* nodeEditor = CPLGetXMLNode(node, "=ShapeEditorClass");
		if (nodeEditor)
		{
			MeasuringHelper::Deserialize(_activeShape, nodeEditor);
			*retVal = VARIANT_TRUE;
		}
		CPLDestroyXMLNode(node);
	}

	return ;
}

#pragma once
class CShapeEditor;
class CGeoProjection;
class CUndoList;


class IMapViewCallback
{
public:
	virtual CShapefile* _GetShapefile(LONG layerHandle) = 0;
	virtual CShapeEditor* _GetShapeEditor() = 0;
	virtual ICallback* _GetGlobalCallback() = 0;
	virtual void _ZoomToShape(long layerHandle, long shapeIndex) = 0;
	virtual CGeoProjection* _GetWgs84Projection() = 0;
	virtual CGeoProjection* _GetMapProjection() = 0;
	virtual CUndoList* _GetUndoList() = 0;
	virtual tkTransformationMode _GetTransformationMode() = 0;
	virtual void _ProjectionToPixel(double projX, double projY, double * pixelX, double * pixelY) = 0;
	virtual void _PixelToProjection(double pixelX, double pixelY, double * projX, double * projY) = 0;
	virtual void _FireBeforeDeleteShape(tkDeleteTarget target, tkMwBoolean* cancel) = 0;
	virtual tkCursorMode _GetCursorMode() = 0;
	virtual void _FireValidateShape(LONG LayerHandle, IDispatch* Shape, tkMwBoolean* Cancel) = 0;
	virtual void _FireAfterShapeEdit(tkUndoOperation action, LONG LayerHandle, LONG ShapeIndex) = 0;
	virtual void _FireShapeValidationFailed(LPCTSTR ErrorMessage) = 0;
	virtual void _ZoomToEditor() = 0;
	virtual void _SetMapCursor(tkCursorMode mode, bool clearEditor) = 0;
	virtual void _Redraw(tkRedrawType redrawType, bool updateTiles, bool atOnce) = 0;
	virtual void _FireUndoListChanged() = 0;
	virtual void _UnboundShapeFinished(CShape* shp) = 0;
	virtual double _GetMouseProjTolerance() = 0;
	virtual void _StartDragging(DraggingOperation operation) = 0;
	virtual void _FireBackgroundLoadingStarted(long taskId, long layerHandle) = 0;
	virtual void _FireBackgroundLoadingFinished(long taskId, long layerHandle, long numFeatures, long numLoaded) = 0;
};



#pragma once
class GroupOperation
{
public:
	static bool Run(tkCursorMode cursorMode, long layerHandle, CShapefile* sf, CShape* polyline, CUndoList* undoList, int& errorCode);
	static bool SplitByPolyline(long layerHandle, CShapefile* sf, vector<long>& indices, CShape* polyline, CUndoList* undoList, int& errorCode);
	static bool ClipByPolygon(tkCursorMode cursorMode, long layerHandle, CShapefile* sf, vector<long>& indices, CShape*polygon, CUndoList* undoList, int& errorCode);
};


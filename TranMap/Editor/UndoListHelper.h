#pragma once
class UndoListHelper
{
public:
	static void AddShapes(CShapefile* sf, vector<CShape*> shapes, long layerHandle, CUndoList* undoList, int sourceShapeIndex, int skipField);
	static void DeleteShapes(CShapefile* sf, vector<long>& deleteList, long layerHandle, CUndoList* undoList);
};


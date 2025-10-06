#include "stdafx.h"
#include "UndoListHelper.h"
#include "ShapefileHelper.h"
#include "UndoList.h"

// ********************************************************************
//		AddShapes()
// ********************************************************************
void UndoListHelper::AddShapes(CShapefile* sf, vector<CShape*> shapes, long layerHandle, CUndoList* undoList, int sourceShapeIndex, int skipField)
{
	VARIANT_BOOL vb;
	for (size_t j = 0; j < shapes.size(); j++)
	{
		long shapeIndex;
		sf->EditAddShape(shapes[j], &shapeIndex);
		ShapefileHelper::CopyAttributes(sf, sourceShapeIndex, shapeIndex, skipField);
		undoList->Add(uoAddShape, layerHandle, shapeIndex, &vb);
		delete shapes[j];
	}
}

// ********************************************************************
//		DeleteShapes()
// ********************************************************************
void UndoListHelper::DeleteShapes(CShapefile* sf, vector<long>& deleteList, long layerHandle, CUndoList* undoList)
{
	VARIANT_BOOL vb;
	for (int i = deleteList.size() - 1; i >= 0; i--)
	{
		undoList->Add(uoRemoveShape, layerHandle, deleteList[i], &vb);
		if (vb) {
			sf->EditDeleteShape(deleteList[i], &vb);
		}
	}
}
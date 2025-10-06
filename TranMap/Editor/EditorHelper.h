#pragma once
#include "ShapeEditor.h"
class EditorHelper
{
public:
	static ShpfileType GetShapeType2D(CShapeEditor* editor);
	static bool IsEmpty(CShapeEditor* editor);
	static bool IsGroupOverlayCursor(tkCursorMode cursorMode);
	static bool IsDigitizingCursor(tkCursorMode mode);
	static void CopyOptionsFrom(CShapeEditor* editor, CShapefile* sf);
	static void CopyOptionsFrom(CShapeEditor* editor, CShapeDrawingOptions* options);
	static bool OnCursorChanged(CShapeEditor* editor, bool clearEditor, tkCursorMode newCursor, bool& redrawNeeded);
	static tkLayerSelection GetSnappingBehavior(CShapeEditor* editor);
	static bool IsSnappableCursor(tkCursorMode mode);
};


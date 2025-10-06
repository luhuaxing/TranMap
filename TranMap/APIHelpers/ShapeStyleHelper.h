#pragma once
class ShapeStyleHelper
{
public:
	static void ApplyRandomDrawingOptions(CShapefile* sf);
	static bool GetPointBounds(CShapefile* sf, Extent& extents);
	static bool GetPointBounds(CShapeDrawingOptions* options, Extent& extents);
	static CShapeDrawingOptions* GetHotTrackingStyle(CShapefile* sf, OLE_COLOR outlineColor, bool identify);
	static CStringW GetSymbologyFileAsXml(CShapefile* sf);
};


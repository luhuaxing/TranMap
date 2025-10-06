#pragma once
#include "ActiveShape.h"
#include "Measuring.h"

class MeasuringHelper
{
public:
	static bool OnCursorChanged(CMeasuring* measuring, tkCursorMode newCursor);
	static CPLXMLNode* Serialize(ActiveShape* activeShape, CString elementName);
	static bool Deserialize(ActiveShape* activeShape, CPLXMLNode* node);
};


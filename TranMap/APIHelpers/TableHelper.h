#pragma once
#include "Table.h"

class TableHelper
{
public:
	static long GetNumRows(CTableClass* table);
	static void SetFieldValue(CTableClass* table, int rowIndex, CExpression& expr);
};


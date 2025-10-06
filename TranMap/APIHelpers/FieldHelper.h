#pragma once
class FieldHelper
{
public:
	static bool FieldsAreEqual(CField* field1, CField* field2);
	static bool UniqueFieldNames(CShapefile* sf);
	static long FindNewShapeID(CShapefile* sf, long FieldIndex);
};


#include "stdafx.h"
#include "GeoProcessing.h"
#include "ShapefileHelper.h"
#include "FieldHelper.h"
#include "Shapefile.h"

// ****************************************************************
//		CopyFields()
// ****************************************************************
void GeoProcessing::CopyFields(CShapefile* sfSubject, CShapefile* sfOverlay, CShapefile* sfResult, map<long, long>& fieldMap, bool mergeFields)
{
	// fields of the subject shapefile
	LONG numFields, position;
	VARIANT_BOOL vbretval;
	sfSubject->get_NumFields(&numFields);

	ShapefileHelper::CopyFields(sfSubject, sfResult);

	// passing the fields of overlay shapefile
	if (sfOverlay)
	{
		LONG numFields2;
		sfOverlay->get_NumFields(&numFields2);
		for (long i = 0; i < numFields2; i++)
		{
			CField * field1 = NULL;
			sfOverlay->get_Field(i, &field1);

			// checking whether we have such field already
			bool found = false;
			if (mergeFields)
			{
				for (int j = 0; j < numFields; j++)
				{
					CField * field2 = NULL;
					sfResult->get_Field(j, &field2);
					if (FieldHelper::FieldsAreEqual(field1, field2))
					{
						fieldMap[i] = j;
						found = true;
					}
					delete field2;
				}
			}

			if (!found)
			{
				CField* fieldNew = NULL;
				field1->Clone(&fieldNew);
				sfResult->get_NumFields(&position);
				sfResult->EditInsertField(fieldNew, &position, NULL, &vbretval);
				delete fieldNew;

				fieldMap[i] = position;
			}
			delete field1;
		}
		FieldHelper::UniqueFieldNames(sfResult);
	}
}


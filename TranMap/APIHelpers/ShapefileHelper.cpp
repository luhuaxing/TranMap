#include "stdafx.h"
#include "ShapefileHelper.h"
#include "Shapefile.h"
#include "FieldHelper.h"

// *****************************************************
//		GetMWShapeIdIndex()
// *****************************************************
long ShapefileHelper::GetMWShapeIdIndex(CShapefile* sf)
{
	long index = -1;
	CComBSTR bstr("MWShapeID");
	sf->get_FieldIndexByName(bstr, &index);
	return index;
}

// *****************************************************
//		CopyAttributes()
// *****************************************************
void ShapefileHelper::CopyAttributes(CShapefile* sf, long sourceIndex, long targetIndex, long skipFieldIndex)
{
	long numFields;
	sf->get_NumFields(&numFields);
	CComVariant var;
	VARIANT_BOOL vb;
	for (int i = 0; i < numFields; i++)
	{
		if (i == skipFieldIndex) continue;
		sf->get_CellValue(i, sourceIndex, &var);
		sf->EditCellValue(i, targetIndex, var, &vb);
	}
}

// ***********************************************************************
//		CloneNoFields()
// ***********************************************************************
void ShapefileHelper::CloneNoFields(CShapefile* sf, CShapefile** retVal, bool addShapeId)
{
	if (!sf) {
		*retVal = NULL;
		return;
	}
	CloneNoFields(sf, retVal, GetShapeType(sf), addShapeId);
}

// ***********************************************************************
//		CloneNoFields()
// ***********************************************************************
void ShapefileHelper::CloneNoFields(CShapefile* sfSource, CShapefile** retVal, ShpfileType targetShapeType, bool addShapeId)
{
	CShapefile* sf = new CShapefile();
	VARIANT_BOOL vb;
	if (addShapeId)
	{
		sf->CreateNewWithShapeID(m_globalSettings.emptyBstr, targetShapeType, &vb);
	}
	else {
		sf->CreateNew(m_globalSettings.emptyBstr, targetShapeType, &vb);
	}

	// copying the projection string
	CComBSTR pVal;
	sfSource->get_Projection(&pVal);
	if (pVal != NULL)
		sf->put_Projection(pVal);

	ICallback* callback = NULL;
	sfSource->get_GlobalCallback(&callback);
	sf->put_GlobalCallback(callback);

	*retVal = sf;
}

// ***********************************************************************
//		CloneCore()
// ***********************************************************************
void ShapefileHelper::CloneCore(CShapefile* sfSource, CShapefile** retVal, ShpfileType shpType, bool addShapeId)
{
	ShapefileHelper::CloneNoFields(sfSource, retVal, shpType, addShapeId);
	VARIANT_BOOL vbretval;

	CShapefile* sf = *retVal;

	long numFields;
	sfSource->get_NumFields(&numFields);

	for (long i = 0; i < numFields; i++)
	{
		CField * fld = NULL;
		CField * fldNew = NULL;
		sfSource->get_Field(i, &fld);
		fld->Clone(&fldNew);
		delete fld;

		sf->EditInsertField(fldNew, &i, NULL, &vbretval);
		delete fldNew;

		if (!vbretval)
		{
			sf->Close(&vbretval);
			delete sf;
			sf = NULL;
			break;
		}
	}
}

// *****************************************************************
//		GetSelectedExtents()
// *****************************************************************
bool ShapefileHelper::GetSelectedExtents(CShapefile* sf, double& xMinRef, double& yMinRef, double& xMaxRef, double& yMaxRef)
{
	double xMin, xMax, yMin, yMax;
	bool found = false;
	long numShapes;
	sf->get_NumShapes(&numShapes);
	VARIANT_BOOL vb;

	for (long i = 0; i < numShapes; i++)
	{
		sf->get_ShapeSelected(i, &vb);
		if (vb)
		{
			if (((CShapefile*)sf)->QuickExtentsCore(i, &xMin, &yMin, &xMax, &yMax))
			{
				if (!found)
				{
					xMinRef = xMin, xMaxRef = xMax;
					yMinRef = yMin, yMaxRef = yMax;
					found = true;
				}
				else
				{
					if (xMin < xMinRef)	xMinRef = xMin;
					if (xMax > xMaxRef)	xMaxRef = xMax;
					if (yMin < yMinRef)	yMinRef = yMin;
					if (yMax > yMaxRef)	yMaxRef = yMax;
				}
			}
		}
	}
	return found;
}

// ****************************************************************
//		CopyFields()
// ****************************************************************
void ShapefileHelper::CopyFields(CShapefile* source, CShapefile* target)
{
	if (!target || !target)
		return;

	LONG numFields, position;
	VARIANT_BOOL vbretval;
	source->get_NumFields(&numFields);

	for (long i = 0; i < numFields; i++)
	{
		CField * field = NULL;
		CField * fieldNew = NULL;
		source->get_Field(i, &field);
		field->Clone(&fieldNew);

		target->get_NumFields(&position);
		target->EditInsertField(fieldNew, &position, NULL, &vbretval);

		delete field;
		delete fieldNew;
	}
}

// *****************************************************************
//		GetSelectedIndices()
// *****************************************************************
vector<int>* ShapefileHelper::GetSelectedIndices(CShapefile* sf)
{
	if (!sf) return NULL;
	VARIANT_BOOL vb;
	long numShapes;
	sf->get_NumShapes(&numShapes);
	vector<int>* result = new vector<int>();
	for (long i = 0; i < numShapes; i++)
	{
		sf->get_ShapeSelected(i, &vb);
		if (vb) {
			result->push_back(i);
		}
	}
	return result;
}

// *****************************************************************
//		GetNumShapes()
// *****************************************************************
long ShapefileHelper::GetNumShapes(CShapefile* sf)
{
	if (!sf) return 0;
	long numShapes;
	sf->get_NumShapes(&numShapes);
	return numShapes;
}

// *****************************************************************
//		GetNumFields()
// *****************************************************************
long ShapefileHelper::GetNumFields(CShapefile* sf)
{
	if (!sf) return 0;
	long numFields;
	sf->get_NumFields(&numFields);
	return numFields;
}

// *****************************************************************
//		GetNumSelected()
// *****************************************************************
long ShapefileHelper::GetNumSelected(CShapefile* sf)
{
	if (!sf) return 0;
	long numSelected;
	sf->get_NumSelected(&numSelected);
	return numSelected;
}

// *****************************************************************
//		ShapeSelected()
// *****************************************************************
bool ShapefileHelper::ShapeSelected(CShapefile* sf, int shapeIndex)
{
	if (!sf) return false;
	VARIANT_BOOL vb;
	sf->get_ShapeSelected(shapeIndex, &vb);
	return vb ? true: false;
}

// *****************************************************************
//		Rotate()
// *****************************************************************
void ShapefileHelper::Rotate(CShapefile* sf, double originX, double originY, double angleDegree)
{
	if (!sf) return;
	tkShapefileSourceType sourceType;
	sf->get_SourceType(&sourceType);
	if (sourceType != sstInMemory) return;
	
	long numShapes = GetNumShapes(sf);
	for (long i = 0; i < numShapes; i++)
	{
		CShape* shp = NULL;
		sf->get_Shape(i, &shp);
		if (shp) {
			shp->Rotate(originX, originY, angleDegree);
		}
	}
}

// *****************************************************************
//		GetShapeType2D()
// *****************************************************************
ShpfileType ShapefileHelper::GetShapeType2D(CShapefile* sf)
{
	if (!sf) return SHP_NULLSHAPE;
	ShpfileType shpType;
	sf->get_ShapefileType2D(&shpType);
	return shpType;
}

// *****************************************************************
//		GetShapeType2D()
// *****************************************************************
ShpfileType ShapefileHelper::GetShapeType(CShapefile* sf)
{
	if (!sf) return SHP_NULLSHAPE;
	ShpfileType shpType;
	sf->get_ShapefileType(&shpType);
	return shpType;
}


// *****************************************************************
//		CloneSelection()
// *****************************************************************
CShapefile* ShapefileHelper::CloneSelection(CShapefile* sf)
{
	CShapefile* sfNew = NULL;
	sf->Clone(&sfNew);
	long numShapes, shapeIndex;
	sf->get_NumShapes(&numShapes);
	for (long i = 0; i < numShapes; i++)
	{
		if (!ShapeSelected(sf, i)) continue;
		CShape* shp = NULL;
		sf->get_Shape(i, &shp);
		if (shp) {
			sfNew->EditAddShape(shp, &shapeIndex);
		}
	}
	return sfNew;
}

// *****************************************************************
//		GetClosestPoint()
// *****************************************************************
bool ShapefileHelper::GetClosestPoint(CShapefile* sf, double x, double y, double maxDistance, std::vector<long>& ids, long* shapeIndex, long* pointIndex, double& dist)
{
	if (!sf) return false;

	VARIANT_BOOL vb;
	double minDist = DBL_MAX;
	for (size_t i = 0; i < ids.size(); i++)
	{
		VARIANT_BOOL visible;
		sf->get_ShapeVisible(ids[i], &visible);
		if (!visible) continue;

		CShape* shp = NULL;
		sf->get_Shape(ids[i], &shp);
		if (shp)
		{
			long numPoints;
			shp->get_NumPoints(&numPoints);
			double xPnt, yPnt;
			for (long j = 0; j < numPoints; j++)
			{
				shp->get_XY(j, &xPnt, &yPnt, &vb);
				double dist = sqrt(pow(x - xPnt, 2.0) + pow(y - yPnt, 2.0));
				if (dist < minDist && dist < maxDistance)
				{
					minDist = dist;
					*shapeIndex = ids[i];
					*pointIndex = j;
				}
			}
			delete shp;
		}
	}
	dist = minDist;
	return minDist < maxDistance;
}

// ********************************************************************
//		PointInPolygon()
// ********************************************************************
bool ShapefileHelper::PointInPolygon(CShapefile* sf, long ShapeIndex, double x, double y)
{
	if (!sf) return false;
	VARIANT_BOOL InPolygon;
	if (ShapeIndex < 0) return FALSE;
	sf->PointInShape(ShapeIndex, x, y, &InPolygon);
	return InPolygon ? true : false;
}

/***********************************************************************/
/*		BoundsWithinPolygon()
/***********************************************************************/
bool ShapefileHelper::BoundsWithinPolygon(CShapefile* sf, int shapeIndex, double b_minX, double b_minY, double b_maxX, double b_maxY)
{
	if (!sf) return false;

	if (ShapefileHelper::PointInPolygon(sf, shapeIndex, b_minX, b_minY))
	{
		return true;
	}
	else if (ShapefileHelper::PointInPolygon(sf, shapeIndex, b_maxX, b_maxY))
	{
		return true;
	}
	else if (ShapefileHelper::PointInPolygon(sf, shapeIndex, b_minX, b_maxY))
	{
		return true;
	}
	else if (ShapefileHelper::PointInPolygon(sf, shapeIndex, b_maxX, b_minY))
	{
		return true;
	}
	return false;
}

// ********************************************************************
//		ShapeTypeIsM()
// ********************************************************************
bool ShapefileHelper::ShapeTypeIsM(CShapefile* sf)
{
	if (!sf) return false;
	ShpfileType shpType = GetShapeType(sf);
	return Utility::ShapeTypeIsM(shpType);
}

// ********************************************************************
//		GetSymbologyFilename()
// ********************************************************************
CStringW ShapefileHelper::GetSymbologyFilename(CShapefile* sf)
{
	if (!sf) return L"";
	CComBSTR bstr;
	sf->get_Filename(&bstr);
	CStringW name = OLE2W(bstr);
	if (name.GetLength() > 0)
	{
		name += L".mwsymb";
		if (Utility::FileExistsUnicode(name))
			return name;
	}
	return L"";
}

// ********************************************************************
//		InteractiveEditing()
// ********************************************************************
bool ShapefileHelper::InteractiveEditing(CShapefile* sf)
{
	if (!sf) return false;
	VARIANT_BOOL editing;
	sf->get_InteractiveEditing(&editing);
	return editing ? true : false;
}

// ********************************************************************
//		IsVolatile()
// ********************************************************************
bool ShapefileHelper::IsVolatile(CShapefile* sf)
{
	if (!sf) return false;
	VARIANT_BOOL isVolatile;
	sf->get_Volatile(&isVolatile);
	return isVolatile ? true : false;
}

// ********************************************************************
//		InsertMwShapeIdField()
// ********************************************************************
void ShapefileHelper::InsertMwShapeIdField(CShapefile* sf)
{
	if (!sf) return;

	CField* shapeIDField = new CField();
	CComBSTR bstr("MWShapeID");
	shapeIDField->put_Name(bstr);
	shapeIDField->put_Type(INTEGER_FIELD);
	shapeIDField->put_Width(10);
	shapeIDField->put_Precision(10);

	long fldIndex = 0;
	VARIANT_BOOL retVal;
	sf->EditInsertField(shapeIDField, &fldIndex, NULL, &retVal);
}

// ********************************************************************
//		GetSourceType()
// ********************************************************************
tkShapefileSourceType ShapefileHelper::GetSourceType(CShapefile* sf)
{
	if (!sf) return sstUninitialized;
	tkShapefileSourceType type;
	sf->get_SourceType(&type);
	return type;
}

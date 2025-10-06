#include "StdAfx.h"
#include "HotTrackingInfo.h"
#include "ShapefileHelper.h"
#include "Shapefile.h"
#include "ShapeStyleHelper.h"

// ************************************************************
//		Update
// ************************************************************
void HotTrackingInfo::Update(CShapefile* source, CShape* shp, int layerHandle, int shapeIndex)
{
	VARIANT_BOOL vb;
	if (!Shapefile) {
		Shapefile = new CShapefile();
	}
	else {
		Shapefile->Close(&vb);
	}

	ShpfileType type = ShapefileHelper::GetShapeType(source);

	((CShapefile*)Shapefile)->CreateNewCore(m_globalSettings.emptyBstr, type, false, &vb);

	long index = 0;
	Shapefile->EditInsertShape(shp, &index, &vb);

	Shapefile->RefreshExtents(&vb);
	LayerHandle = layerHandle;
	ShapeIndex = shapeIndex;
}


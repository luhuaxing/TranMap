#pragma once
#include "ogr_feature.h"

class Shape2Ogr
{
public:
	static bool Shapefile2OgrLayer(CShapefile* sf, OGRLayer* poLayer, bool saveLabels, ICallback* callback = NULL);
	static int SaveShapefileChanges(OGRLayer* poLayer, CShapefile* shapefile, long shapeCmnIndex, tkOgrSaveType saveType, bool validateShapes, vector<OgrUpdateError>& errors);
	static bool ShapeRecord2Feature(CShapefile* sf, long shapeIndex, OGRFeature* feature, OGRFeatureDefn* fields, tkOgrSaveType saveType, bool editing, CString& validationError, bool validateShape = false);
	static int RemoveDeletedFeatures(OGRLayer* layer, CShapefile* sf, long shapeCmnIndex);
private:
	static bool ShapefileFieldsToOgr(CShapefile* sf, OGRLayer* poLayer);
	static void ShapesToOgr(CShapefile* sf, OGRLayer* layer, ICallback* callback);
};


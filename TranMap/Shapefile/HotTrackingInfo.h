#include "ShapeDrawingOptions.h"
#include "Shapefile.h"
class HotTrackingInfo
{
public:
	CShapefile* Shapefile;
	int ShapeIndex;
	int LayerHandle;
	
	HotTrackingInfo() : Shapefile(NULL), ShapeIndex(-1), LayerHandle(-1) {}
	~HotTrackingInfo()
	{
		if (Shapefile)
		{
			VARIANT_BOOL vb;
			Shapefile->Close(&vb);
			//Shapefile->Release();
		}
	}
	bool IsEmpty()
	{
		return ShapeIndex == -1;
	}
	bool IsSame(LayerShape& info) {
		return LayerHandle == info.LayerHandle && ShapeIndex == info.ShapeIndex;
	}
	void Clear() 
	{
		VARIANT_BOOL vb;
		ShapeIndex = -1;
		LayerHandle = -1;
		if (Shapefile)
			Shapefile->Close(&vb);
	}
	void Update(CShapefile* source, CShape* shp, int layerHandle, int shapeIndex);
	void UpdateStyle(CShapeDrawingOptions* options)
	{
		if (Shapefile) {
			Shapefile->put_DefaultDrawingOptions(options);
		}
	}
};

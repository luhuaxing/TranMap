

#pragma once
#include "BaseDrawer.h"
#include "Shapefile.h"
#include "LinePattern.h"
#include "ShapefileReader.h"
#include "DrawingOptions.h"
#include "CollisionList.h"
#include "Shape.h"

class CShapefileDrawer
{
public:
	CShapefileDrawer(Gdiplus::Graphics* graphics, Extent* extents, double dx, double dy, CCollisionList* collisionList, double scale, bool forceGdiplus = false)
	{
		m_hdc = NULL;
		_dc = NULL;			// should be obtained from Graphics and released after the usage
		_shapefile = NULL;
		_extents = extents;
		_dx = dx;
		_dy = dy;
		_sfReader = NULL;
		_xMin = 0.0;
		_yMin = 0.0;
		_xMax = 0.0;
		_yMax = 0.0;
		_shptype = SHP_NULLSHAPE;
		
		_avoidCollisions = true;

		_isEditing = VARIANT_FALSE;
		_fastMode = VARIANT_FALSE;

		_shapeData = NULL;

		_collisionList = collisionList;
		_forceGdiplus = forceGdiplus;

		m_selectionColor = RGB(255, 255, 0);
		m_selectionTransparency = 180;

		_graphics = graphics;
		_graphics->SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

		_bmpPixel = new Gdiplus::Bitmap(1, 1);

		_scale = scale;
		_shapeCount = 0;
	};

	~CShapefileDrawer()
	{
		for (unsigned long i = 0; i < _pointRectangles.size(); i++)
			delete _pointRectangles[i];
		_pointRectangles.clear();
		delete _bmpPixel;
	};

protected:
	double _scale;
	Gdiplus::Graphics* _graphics;
	Gdiplus::Bitmap* _bmpPixel;
	
	CDC* _dc;	// should be released after each usage
	HDC m_hdc;


	OLE_COLOR m_selectionColor;
	unsigned short m_selectionTransparency;
	std::vector<ShapeData*>* _shapeData;		// reference for shapefile's data for faster access
	std::vector<CRect*> _pointRectangles;
	CShapefile* _shapefile;
	Extent* _extents; 
	double _dx;
	double _dy;
	CShapefileReader* _sfReader;
	int _shapeCount;
	
	double _xMin;
	double _yMin;
	double _xMax;
	double _yMax;
	
	ShpfileType _shptype;
	bool _avoidCollisions;
	VARIANT_BOOL _isEditing;
	VARIANT_BOOL _fastMode;
	CCollisionList* _collisionList;
	CCollisionList _localCollisionList;
	bool _forceGdiplus;
	
	struct VertexPath
	{
		Gdiplus::GraphicsPath* path;
		CDrawingOptionsEx* options;
	};

	std::vector<VertexPath> _vertexPathes;
public:
	int Draw(const CRect & rcBounds, CShapefile* sf);
	int GetShapeCount() { return _shapeCount; }
private:	
	std::vector<long>* SelectShapesFromSpatialIndex(char* sFilename, Extent* extents);
	
	// GDI drawing
	void DrawLineCategoryGDI( CDrawingOptionsEx* options, std::vector<int>* indices, bool drawSelection);
	void DrawPolyGDI(PolygonData* shapeData, CDrawingOptionsEx* options, Gdiplus::GraphicsPath& path, bool pathIsNeeded );
	void DrawPolyGDI(IShapeData* shp, CDrawingOptionsEx* options, Gdiplus::GraphicsPath& path, bool pathIsNeeded );
	void DrawPolyGDI(CShapeWrapperCOM* shp, CDrawingOptionsEx* options, Gdiplus::GraphicsPath& path, bool pathIsNeeded );
	inline void DrawPolygonPoint(double &xMin, double& xMax, double& yMin, double& yMax, OLE_COLOR& pointColor);

	// GDI+ drawing
	void DrawPolyCategory(CDrawingOptionsEx* options, std::vector<int>* indices, bool drawSelection);
	bool DrawPolygonGDIPlus(int shapeIndex, Gdiplus::GraphicsPath& path, double minSize, OLE_COLOR pointColor, tkVectorDrawingMode drawingMode,
										 double& xMin, double& xMax, double& yMin, double& yMax);

	void AddPolygonToPath( Gdiplus::GraphicsPath* pathFill, IShapeData* shp, tkVectorDrawingMode drawingMode);
	void AddPolygonToPath( Gdiplus::GraphicsPath* pathFill, PolygonData* shapeData, tkVectorDrawingMode drawingMode);
	void AddPolygonToPath( Gdiplus::GraphicsPath* pathFill, CShapeWrapperCOM* shp, tkVectorDrawingMode drawingMode);
	
	// drawing of point layer
	void DrawPointCategory( CDrawingOptionsEx* options, std::vector<int>* indices, bool drawSelection);
	void DrawVertices(Gdiplus::GraphicsPath* path, CDrawingOptionsEx* options);

	// line pattern
	void DrawLinePatternCategory(CDrawingOptionsEx* options, std::vector<int>* indices, bool drawSelection);
	void DrawPolylinePath(Gdiplus::GraphicsPath* path, CDrawingOptionsEx* options, bool drawSelection);

	void DrawCategory(CDrawingOptionsEx* options, std::vector<int>* indices, bool drawSelection);

	bool WithinVisibleExtents(double xMin, double xMax, double yMin, double yMax)
	{
		bool result = !(xMin > _extents->right || xMax < _extents->left || yMin > _extents->top || yMax < _extents->bottom);
		if (result) _shapeCount++;
		return result;
	};

	void InitDC()
	{
		if (!_dc)
		{
			m_hdc = _graphics->GetHDC();
			_dc = CDC::FromHandle(m_hdc);
		}
	}
	void ReleaseDC()
	{
		if (_dc)
		{
			_graphics->ReleaseHDC(m_hdc);
			_dc = NULL;
		}
	}
	//bool HavePointCollision(CRect* rect);
};
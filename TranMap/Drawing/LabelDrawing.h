#pragma once
#include "LabelOptions.h"
#include "CollisionList.h"
#include "BaseDrawer.h"

class CLabelDrawer: public CBaseDrawer
{
public:
	// constructor for spatially referenced labels
	CLabelDrawer(Gdiplus::Graphics* graphics, Extent* extents, double pixelPerProjectionX, double pixelPerProjectionY, 
										double currentScale, CCollisionList* collisionList, double mapRotation, bool printing);

	// constructor for screen referenced labels
	CLabelDrawer(Gdiplus::Graphics* graphics, Extent* extents, CCollisionList* collisionList, double mapRotation);
	~CLabelDrawer(void){};

private:	
	HDC m_hdc;
	double _currentScale;
	double _mapRotation;
	CCollisionList* _collisionList;
	bool _spatiallyReferenced;
	bool _printing;
public:
	void DrawLabels( CLabels* LabelsClass);
	
private:
	inline void AlignRectangle(CRect& rect, tkLabelAlignment alignment);
	bool HaveCollision(CRotatedRectangle& rect);
	bool HaveCollision(CRect& rect);
	void DrawLabelFrameGdiPlus(Gdiplus::Graphics& graphics, Gdiplus::Brush* brush, Gdiplus::Pen& pen, Gdiplus::RectF& rect, CLabelOptions* m_options);
	void DrawLabelFrameGdi(CDC* dc,  CRect* rect, CLabelOptions* m_options );
	inline UINT AlignmentToGDI(tkLabelAlignment alignment);
	inline void AlignmentToGDIPlus(tkLabelAlignment alignment, Gdiplus::StringFormat& format);
	void CalcRotation(CLabelInfo* lbl, double& angle);
};

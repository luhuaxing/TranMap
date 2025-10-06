

#pragma once

// ***********************************************************
//		Structure CRotatedRect
// ***********************************************************
// a class to manipulate data of label bounds
class CRotatedRectangle
{
public:	
	// constructors
	CRotatedRectangle(long& x1, long& y1, long& x2, long& y2, long& x3, long& y3, long& x4, long& y4);
	CRotatedRectangle(CRect& rect);
	CRotatedRectangle(){boxCalculated = false;};

	// destructor
	virtual ~CRotatedRectangle()
	{
	}
	
	// functions
	bool Intersects(CRotatedRectangle& rect);
	bool Intersects(CRect& rect);
	bool BoundsIntersect(CRotatedRectangle& rect);
	bool BoundsIntersect(CRect& rect);
	CRect* BoundingBox();
	POINT points[4];		// data
private:
	CRect box;
	bool boxCalculated;	// if we calculted rectangle, it will be cached in rect variable
};

#pragma once
#include "RotatedRectangle.h"

class CCollisionList
{
public:
	CCollisionList(void){}
	~CCollisionList(void)
	{
		this->Clear();
	}

	// members
	std::vector<CRect*> _extentsHorizontal;			 // extents for labels w/o rotation
	std::vector<CRotatedRectangle*> _extentsRotated; // extents for labels with rotation

	// functions
public:
	bool CCollisionList::HaveCollision(CRect& rect);
	bool CCollisionList::HaveCollision(CRotatedRectangle& rect);
	void Clear();
	void AddRectangle(CRect* rect, int bufferX = 0, int bufferY = 0);
	void AddRotatedRectangle(CRotatedRectangle* rect, int bufferX = 0, int bufferY = 0);
};


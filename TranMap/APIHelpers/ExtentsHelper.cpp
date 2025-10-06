#include "stdafx.h"
#include "ExtentsHelper.h"


// ****************************************************************
//		Populate()
// ****************************************************************
CExtents* ExtentsHelper::Populate(Extent& source)
{
	CExtents* box = new CExtents();
	box->SetBounds(source.left, source.bottom, 0.0, source.right, source.top, 0.0);
	return box;
}

// ****************************************************************
//		GetWorldBounds()
// ****************************************************************
CExtents* ExtentsHelper::GetWorldBounds()
{
	CExtents* bounds = new CExtents();
	bounds->SetBounds(-179.5, -85.0, 0.0, 179.5, 85.0, 0.0);
	return bounds;
}

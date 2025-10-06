
#include "stdafx.h"
#include "TileCore.h"

// gets relative path of tile disk storage
CStringW TileCore::getPath(CStringW root, CStringW ext)
{
	CStringW path;
	path.Format(L"%s%d\\%d\\%d%s", root, this->m_scale, this->m_tileX, this->m_tileY, ext);
	return path;
}


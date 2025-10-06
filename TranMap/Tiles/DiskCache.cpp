
#include "stdafx.h"
#include "DiskCache.h"

bool DiskCache::init = false;
CLSID DiskCache::pngClsid;
CStringW DiskCache::rootPath;	// root folder for storage
CStringW DiskCache::ext;			// extension (with leading point)
CString DiskCache::encoder;		// gdi+ encoder


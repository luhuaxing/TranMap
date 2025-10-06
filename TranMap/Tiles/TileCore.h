
#pragma once
#include "InMemoryBitmap.h"
#include "GeoPoint.h"
#include "CustomProjection.h"

// Represents a single map tile - image or several overlayed images
// with specified zoom and X, Y coordinates, usually with size 256 by 256 pixels
class TileCore
{
public:
	// the code is used by clients via COM interfaces so 
	// no need to bother much about encapsulation

	bool m_hasErrors;		// there were errors during download, one of several layers weren't loaded

	bool m_drawn;			// it's drawn on screen
	bool m_toDelete;		// for inner working of RAM cache
	
	bool m_inBuffer;		// it's currently displayed or scheduled to be displayed; it must not be destroyed while cleaning the cache

	long m_scale;
	long m_tileX;
	long m_tileY;
	int m_providerId;
	
	// geographic and projected coordinates
	RectLatLng Geog;
	RectLatLng Proj;

	int _projectionChangeCount;	// position of tile was recalculated using the current projection; number of projection changes
	BaseProjection* m_projection;
	
	CStringW getPath(CStringW root, CStringW ext);

	// a tile may be comprised of several semi-transparent bitmaps (e.g. satellite image and labels above it)
	vector<CMemoryBitmap*> Overlays;

#pragma region Construtor/destrutor
	
	// Constructor
	TileCore(int providerId, int zoom, CPoint& pnt, BaseProjection* projection)
		: m_scale(zoom), m_tileX(pnt.x), m_tileY(pnt.y), m_providerId(providerId), m_projection(projection)
	{
		m_refCount = 0;
		m_drawn = false;
		m_toDelete = false;
		_projectionChangeCount = -1;
		m_hasErrors = false;
		Geog =	projection->CalculateGeogBounds(pnt, zoom);
	}

	~TileCore()
	{
		ClearOverlays();
	}
#pragma endregion

#pragma region Overlays
	// clears overlays and releases memory
	void ClearOverlays()
	{
		for (size_t i = 0; i < Overlays.size(); i++)
		{
			delete Overlays[i];
		}
		Overlays.clear();
	}

	bool IsEmpty() 	{ return Overlays.size() == 0; }
	
	// returns combined size of bitmaps for all overlays
	int getSize()
	{
		int size = 0;
		for (size_t i = 0; i < Overlays.size(); i++)
		{
			size += Overlays[i]->get_Size();
		}
		return size;
	}

	// adds a single bitmap or several bitmaps in case of complex provider
	void AddBitmap(CMemoryBitmap* bitmap) 
	{	
		Overlays.push_back(bitmap);	
	}

	// get specified overlay
	CMemoryBitmap* getBitmap(int index)
	{
		return (index >= 0 && index < (int)Overlays.size()) ? Overlays[index] : NULL;
	}
#pragma endregion
	
#pragma region Reference counting
private:
	long m_refCount;		// number of references (it can be used in drawing, RAM cache or be scheduled for disk caching)
							// a tile can be deleted while:
							// 1) clearing drawing buffer: Tiles::Clear()
							// 2) clearing RAM cache: RAMCache::Clear()
							// 3) after disk caching: SQLiteCache::DoCaching()
public:
	long AddRef()
	{
		InterlockedIncrement(&m_refCount);
		return m_refCount;
	}
	
	// Attention: object is deleted automatically when reference count is equal to 0
	long Release()
	{
		InterlockedDecrement(&m_refCount);
		
		if (m_refCount < 0)
			CallbackHelper::AssertionFailed("Invalid reference count for a tile.");

		if (this->m_refCount == 0)
		{
			delete this;
			return 0;
		}
		else {
			return m_refCount;
		}
	}
#pragma endregion

	bool TileCore::operator==(const TileCore &t2)
	{
		return (this->m_tileX == t2.m_tileX && 
				this->m_tileY == t2.m_tileY &&
				this->m_scale == t2.m_scale &&
				this->m_providerId == t2.m_providerId);
	}

};

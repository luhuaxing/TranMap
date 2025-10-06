
#pragma once
#include "TileCore.h"
using namespace std;

// provider id -> zoom -> x -> y
typedef std::map<int, TileCore*>					TilePositions;   // indexed by y
typedef std::map<int, TilePositions*>				TilePoints;		 // indexed by x
typedef std::map<int, std::vector<TilePoints*>*>	TilesCache;		 // indexed by provider; indices in vector - by zoom 

// Provides storage for map tiles in RAM
class RamCache
{
private:	
	static int m_size;		// size of cache in bytes
	static void DeleteMarkedTiles();	// deletes tiles from chronological vector
public:
	static double m_maxSize;			// max size in megabytes
	static TilesCache m_tilesMap;		// the main storage
	static vector<TileCore*> m_list;	// the list of tiles in the chronological order

	static void AddToCache(TileCore* tile);
	static TileCore* get_Tile(int provider, int scale, int tileX, int tileY);
	static double get_MemorySize(int provider = -1, int scale = -1);
	static void ClearAll(int fromScale, int toScale);
	static void ClearByProvider(int provider = -1, int fromScale = 0, int toScale = 100);
	static void ClearOldest(int sizeToClearBytes);
	static void Close();
};

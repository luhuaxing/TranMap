#pragma once
#include <afxmt.h>
#include <queue>
#include <list>
#include "sqlite3.h"
#include "TileCore.h"
#include "TileCacher.h"
#include "BaseProvider.h"

#define DB_NAME L"mwtiles.db3"

// Provides storage for map tiles in SQLite database
class SQLiteCache
{
	static CStringW m_dbName;
	static sqlite3 *m_conn;
public:
	static ::CCriticalSection section;  // finer locks to prevent simultaneous access of the same operation from several threads
	static bool _createNeeded;	
	static bool _openNeeded;	
	static bool m_locked;		// coarse lock to block the adding tile to cache when extracting operations are made
	static double maxSizeDisk;	// max size of disk cache in megabytes
	
	static CStringW get_DbName();
	static CStringW get_DefaultDbName();
	static bool set_DbName(CStringW name);
	static bool CreateDatabase();
	
	static TileCore* get_Tile(BaseProvider* provider, LONG scale, LONG x, LONG y);
	static void Clear(int providerId = -1, int fromScale = 0, int toScale = 100);
	static double get_FileSize();
	
	static double get_FileSize(tkTileProvider provider, int scale);
	static void AutoClear();
	static int get_TileCount(int provider, int zoom, int xMin, int xMax, int yMin, int yMax);
	static bool get_TilesXY(int provider, int zoom, int xMin, int xMax, int yMin, int yMax, std::list<CPoint*>& list);
	static bool get_Exists(BaseProvider* provider, LONG scale, LONG x, LONG y);

	static void ProcessQueue();
	static void DoCaching(TileCore* tile);
	
	// closes db connection
	static void Close()
	{
		if (m_conn)
		{
			sqlite3_close(m_conn);
			m_conn = NULL;
		}
	}

	static bool Initialize(SqliteOpenMode openMode);
};

	
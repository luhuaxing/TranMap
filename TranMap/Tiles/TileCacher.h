
#pragma once
#include <afxmt.h>
#include <atlutil.h>
#include <queue>
#include "Threading.h"
#include "TileCore.h"

#define POOL_SIZE	1

// Handles the queue requests to add tiles to specific cache provider
class TileCacher
{
private:
	std::queue<TileCore*> queue;
	CThreadPool<ThreadWorker>* myPool;
	bool running;
public:
	bool stopped;
	CacheType cacheType;
	::CCriticalSection queueLock;

	TileCacher()
	{
		this->cacheType = CacheType::SqliteCache;
		myPool = new CThreadPool<ThreadWorker>();
		HRESULT hr = myPool->Initialize((void*)322, POOL_SIZE);
		if (!SUCCEEDED(hr))
			CallbackHelper::ErrorMsg("Failed to initialize thread pool for caching tiles.");
		running = false;
		stopped = false;
	}

	~TileCacher(void)
	{
		if (myPool != NULL) {
			myPool->SetTimeout(50);
			myPool->Shutdown();
			delete myPool;
		}
	}

	void Enqueue(TileCore* tile)
	{
		this->queueLock.Lock();
		queue.push(tile);
		this->queueLock.Unlock();
	}

	void Run();
};

// Represents a single caching task (single tile to write)
class CachingTask: public ITask
{
public:	
	TileCore* tile;
	TileCacher* cacher;
	CacheType cacheType;

	CachingTask(TileCore* tile, TileCacher* cacher, CacheType cacheType)
	{
		this->cacher = cacher;
		this->tile = tile;
		this->cacheType = cacheType;
	}

	void DoTask();
};
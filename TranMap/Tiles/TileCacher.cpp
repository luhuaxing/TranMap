
#include "stdafx.h"
#include "TileCacher.h"
#include "SqliteCache.h"
#include "DiskCache.h"

// Runs caching of appropriate type
void CachingTask::DoTask()
{
	if (!this->cacher->stopped)
	{
		if (cacheType == CacheType::SqliteCache) {
			SQLiteCache::DoCaching(this->tile);
		}
		else {
			DiskCache::DoCaching(tile);
		}
		this->cacher->Run();	// cache the next tile
	}
	delete this;
}

// Queues caching request
void TileCacher::Run()
{
	if (this->cacheType == CacheType::SqliteCache && SQLiteCache::m_locked)
		return;

	this->queueLock.Lock();
	if (!queue.empty())
	{
		TileCore* tile = queue.front();
		queue.pop();
		CachingTask* task = new CachingTask(tile, this, this->cacheType);
		myPool->QueueRequest( (ThreadWorker::RequestType) task );
	}
	this->queueLock.Unlock();
}
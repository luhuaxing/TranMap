
#pragma once

// Represents task for asynchronous execution
class ITask
{
public:
	virtual void DoTask() = 0;
};

// A thread worker to run asyncronous task compatible with ATL CThreadPool  
class ThreadWorker
{
public:
    typedef DWORD_PTR RequestType;
	ThreadWorker() : m_dwExecs( 0 ) {}

    virtual BOOL Initialize(void *pvParam) { return TRUE;}
	virtual void Terminate(void* ) {}
	virtual void Execute(RequestType dw, void *pvParam, OVERLAPPED* pOverlapped) throw()
	{
		ITask* pTask = (ITask*)(DWORD_PTR)dw;
		pTask->DoTask();
		m_dwExecs++;
	}
    virtual BOOL GetWorkerData(DWORD, void **) { return FALSE; }
protected:
	DWORD	m_dwExecs;
}; 

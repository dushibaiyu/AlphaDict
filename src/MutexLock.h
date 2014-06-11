/**
 * A wrapper of mutex.
 *
 * changes:
 *     Feb 19,2014  Created. [liqiong lee]
 *
 */
#ifndef _MUTEXLOCK_H_
#define _MUTEXLOCK_H_

# ifdef _WINDOWS
#include <windows.h>
#define mutex_handle  HANDLE
# else
#include <pthread.h>
#define mutex_handle  pthread_mutex_t
# endif

class MutexCriticalSection {
public:
	MutexCriticalSection(bool re=false);

	~MutexCriticalSection()
	{
        #ifdef _WINDOWS
            CloseHandle(m_mutex);
        #else
	    pthread_mutex_destroy(&m_mutex);
        #endif   
	}

	void lock()
	{
	#ifdef _WINDOWS
            WaitForSingleObject(m_mutex, INFINITE);
        #else
	    pthread_mutex_lock(&m_mutex);
        #endif
	}

        void trylock()
	{
        #ifdef _WINDOWS
            WaitForSingleObject(m_mutex, INFINITE);
	#else
	    pthread_mutex_trylock(&m_mutex);
        #endif
	}

	void unlock()
	{
        #ifdef _WINDOWS
            ReleaseMutex(m_mutex);
	#else 
	    pthread_mutex_unlock(&m_mutex);
        #endif 
	}

	mutex_handle& acquire() {return m_mutex;}

private:
    mutex_handle m_mutex;
};

class MutexLock {
public:
	MutexLock(MutexCriticalSection &mcs);
	~MutexLock();
private:
	MutexCriticalSection& m_criticalSection;
};

#endif

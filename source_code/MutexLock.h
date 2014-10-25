
#include <Windows.h>

#ifndef __MUTEXLOCK_H__
#define __MUTEXLOCK_H__


class MutexLock
{
public:
	MutexLock(HANDLE mutex) 
		:m_mutex(mutex)
	{
		if(!mutex)return;
		DWORD waitResult = WaitForSingleObject( 
            m_mutex,    // handle to mutex
            INFINITE);
	};
	~MutexLock() {
		ReleaseMutex(m_mutex);
	}
private:
	HANDLE m_mutex;
};

#endif
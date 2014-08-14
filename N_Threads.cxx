#include <N_Threads.h>

#ifdef WIN32
// WINDOWS
#include <windows.h>
#include <process.h>
namespace ab {
	void sleep(int n){ Sleep(n); }
	int n_create_thread(Fl_Thread& t, void *(*f) (void *), void* p) {
		return t = (Fl_Thread)_beginthread((void(__cdecl *)(void *))f, 0, p);
	}
	void n_wait_end_thread(Fl_Thread& t)
	{
		WaitForSingleObject((HANDLE)t, INFINITE);
	}

}
#else /*WIN32*/
// UNIX
#include <unistd.h>                            // usleep
#include <pthread.h>
namespace ab {
	inline void sleep(int n){ usleep(n * 1000) }
	static int n_create_thread(Fl_Thread& t, void *(*f) (void *), void* p) {
		return pthread_create((pthread_t*)&t, 0, f, p);
	}

}
#endif /*WIN32*/ 

/* Set up some "thread portability" - this is derived from fltk's own threads.h test stub
* for simplicity, though more complex options exist... */
#include "N_Threads.h"
#ifdef WIN32 /* For WinXX hosts, use _beginthread */

extern DWORD main_thread;
namespace ab {
	Fl_Thread n_create_thread(void(*f) (void *), void *p)
	{
		return (Fl_Thread)_beginthread(f, 0, p);
	}

	void n_wait_end_thread(Fl_Thread t)
	{
		WaitForSingleObject((HANDLE)t, INFINITE);
	}
}

#else /* OSX, linux, and other Unix's - assume pthreads */
namespace ab {
Fl_Thread fl_create_thread(Fl_Thread &t, void *(*f) (void *), void *p) {
	return pthread_create((pthread_t*)&t, 0, f, p);
	join,  pthread_join()
}
}
#endif

#ifndef N_THREADS_H
#define N_THREADS_H
#ifdef WIN32
// WINDOWS
#include <windows.h>
#include <process.h>
namespace ab {
	void sleep(int n);
	typedef unsigned long Fl_Thread;
	int n_create_thread(Fl_Thread& t, void *(*f) (void *), void* p);
	void n_wait_end_thread(Fl_Thread& t);
}
#else /*WIN32*/
// UNIX
#include <unistd.h>                            // usleep
#include <pthread.h>
namespace ab {
	inline void sleep(int n);
	typedef pthread_t Fl_Thread;
	static int n_create_thread(Fl_Thread& t, void *(*f) (void *), void* p)
}
#endif /*WIN32*/ 
#endif /*N_THREADS_H*/
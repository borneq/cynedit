#ifdef WIN32 /* For WinXX hosts, use _beginthread */
#  include <windows.h>
#  include <process.h>

typedef unsigned long Fl_Thread;
namespace ab {
	Fl_Thread n_create_thread(void(*f) (void *), void *p);
	void n_wait_end_thread(Fl_Thread t);
}

#  define PAUSE(x)    Sleep(x) /* delay in milliseconds */
#  define SCHED_YIELD Sleep(0) /* causes a yield on win32 */

#else /* OSX, linux, and other Unix's - assume pthreads */
#  include <pthread.h>
#  include <sched.h>  /* for sched_yield */

typedef pthread_t Fl_Thread;
namespace ab {
	Fl_Thread fl_create_thread(void (*f) (void *), void *p);
}

#  define PAUSE(x)    usleep((x)*1000) /* delay in milliseconds */
#  define SCHED_YIELD sched_yield()
#endif

#ifndef N_Coroutine_h
#define N_Coroutine_h

#ifdef WIN32
#include <Windows.h>
#else
#endif

namespace ab {
#ifdef WIN32
#else
	struct fiber_t
	{
		ucontext_t  fib;
		jmp_buf     jmp;
	};
#endif
	class N_Coroutine {
	private:
#ifdef WIN32
		void *fiber;
    	void *prevfiber;
#else
		fiber_t fib;
#endif
	public:
		virtual void execute() =0;
		void yield(N_Coroutine *next=NULL);
		void attach();
		N_Coroutine(long stackSize);
		virtual ~N_Coroutine();
	};
}
#endif /*N_Coroutine_h*/
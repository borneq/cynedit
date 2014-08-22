#include <Windows.h>
#include <N_Coroutine.h>

#ifdef __GNUC__
  #define threadvar __thread
#else
  #define threadvar __declspec(thread)
#endif

namespace ab {
#ifdef WIN32
threadvar void*  initFiber = NULL;

void __stdcall fiberProc(void *param)
{
	N_Coroutine *coroutine = (N_Coroutine *)param;
	coroutine->execute();
	SwitchToFiber(initFiber);
}

void N_Coroutine::yield(N_Coroutine *next)
{
	if (next==NULL)
		SwitchToFiber(prevfiber);
	else
	{
		next->prevfiber = fiber;
		SwitchToFiber(next->fiber);
	}
}

void N_Coroutine::attach()
{
	SwitchToFiber(fiber);
}

N_Coroutine::N_Coroutine(long stackSize)
{
	if (initFiber==NULL)
		initFiber = ConvertThreadToFiber(NULL);
	fiber = CreateFiber(stackSize, fiberProc, (void*)this);
	prevfiber = initFiber;
}
N_Coroutine::~N_Coroutine()
{
	DeleteFiber(fiber);
}
#else
struct fiber_ctx_t
{
    void(*      fnc)(void*);
    void*       ctx;
    jmp_buf*    cur;
    ucontext_t* prv;
};

static void fiber_start_fnc(void* p)
{
    fiber_ctx_t* ctx = (fiber_ctx_t*)p;
    void* uctx = ctx->ctx;
    if (_setjmp(*ctx->cur) == 0)
    {
        ucontext_t tmp;
        swapcontext(&tmp, ctx->prv);
    }
    Coroutine *coroutine = (Coroutine *)uctx;
    coroutine->execute();
}

inline void create_fiber(fiber_t* fib, void* uctx)
{
    getcontext(&fib->fib);
    size_t const stack_size = 64*1024;
    fib->fib.uc_stack.ss_sp = (::malloc)(stack_size);
    fib->fib.uc_stack.ss_size = stack_size;
    fib->fib.uc_link = 0;
    ucontext_t tmp;
    fiber_ctx_t ctx = {NULL, uctx, &fib->jmp, &tmp};
    makecontext(&fib->fib, (void(*)())fiber_start_fnc, 1, &ctx);
    swapcontext(&tmp, &fib->fib);
}

inline void switch_to_fiber(fiber_t& fiber, fiber_t& prevfiber)
{
    if (_setjmp(prevfiber.jmp) == 0)
        _longjmp(fiber.jmp, 1);
}

__thread fiber_t fibmain;
__thread bool initialized_fibmain = false;

void Coroutine::yield(Coroutine *next)
{
    if (next==NULL)
       switch_to_fiber(fibmain, fib);
    else
       switch_to_fiber(next->fib, fib);
}

void Coroutine::attach()
{
    switch_to_fiber(fib, fibmain);
}

Coroutine::Coroutine(long stackSize)
{
    if (!initialized_fibmain)
    {
        create_fiber(&fibmain, NULL);
        initialized_fibmain = true;
    }
    create_fiber(&fib, this);
}

Coroutine::~Coroutine()
{
    free(fib.fib.uc_stack.ss_sp);
}

#endif
}

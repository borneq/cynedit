#include <FL/fl_draw.H>
#include <CynVirtualView.h>
#include <N_Threads.h>

using namespace ab;

namespace afltk {
	void* thread_func(void* p)
	{
		sleep(5000);
		return NULL;
	}
	/// Constructor.
	CynVirtualView::CynVirtualView(int X, int Y, int W, int H, const char *L) : Fl_Group(X, Y, W, H, L) {
		thread = n_create_thread(thread, thread_func, NULL);
		n_wait_end_thread(thread);
	}

	/// Destructor.
	CynVirtualView::~CynVirtualView() {
	}

	int CynVirtualView::handle(int event)
	{
		int ret = Fl_Group::handle(event);
		return 1;
	}

	void CynVirtualView::draw()
	{
		fl_draw("111", 3, x() + 5, y() + 12);
		fl_draw("222", 3, x() + 5, y() + 16*1+ 12);
		fl_draw("333", 3, x() + 5, y() + 16*2 + 12);
		fl_draw("444", 3, x() + 5, y() + 16*3 + 12);
		fl_draw("555", 3, x() + 5, y() + 16*4 + 12);
	}
}

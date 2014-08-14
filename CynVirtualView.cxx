#include <string>
#include <FL/fl_draw.H>
#include <CynVirtualView.h>
#include <N_Threads.h>

using namespace ab;

namespace afltk {
	void* thread_func(void* p)
	{
		int *cnt = (int *)p;
		while (true)
		{
			(*cnt)++;
			sleep(500);
		}
		return NULL;
	}
	/// Constructor.
	CynVirtualView::CynVirtualView(int X, int Y, int W, int H, const char *L) : Fl_Group(X, Y, W, H, L) {
		cnt = 0;
		thread = n_create_thread(thread, thread_func, &cnt);
		//n_wait_end_thread(thread);
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
		std::string s = std::to_string((long long)cnt);
		fl_draw(s.c_str(), s.size(), x() + 5, y() + 12);
	}
}

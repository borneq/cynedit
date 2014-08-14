#ifndef CYNVIRTUALVIEW_H
#define CYNVIRTUALVIEW_H

#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <N_StringList.h>
#include <N_Threads.h>

using namespace ab;

namespace afltk {
	struct CVV_Thread_Data
	{
		int keep_running;
		int count;
		Fl_Widget *widget;
	};

	class FL_EXPORT CynVirtualView : public Fl_Group {
	protected:
		Fl_Thread thread;
		CVV_Thread_Data exchange_data;
	public:
		N_StringList lines;
		CynVirtualView(int X, int Y, int W, int H, const char *L = 0);
		~CynVirtualView();
		int handle(int event);
		void draw();
	};
}

#endif /*CYNVIRTUALVIEW_H*/

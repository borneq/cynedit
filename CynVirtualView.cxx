#include <string>
#include <FL/fl_draw.H>
#include <CynVirtualView.h>
#include <N_Threads.h>

using namespace ab;

namespace afltk {
	void thread_func(void* p)
	{
		CVV_Thread_Data *data = (CVV_Thread_Data *)p;
		data->count = 0;
		/* This is the thread task */
		while ((data->keep_running))
		{
			data->count++;
			Fl::lock(); /* acquire fltk GUI lock */
			data->widget->redraw();
			Fl::unlock(); /* release fltk lock */
			Fl::awake();
			PAUSE(100);
		}
	}

	void Scrollbar_CB(Fl_Widget*, void *p) {

	}

	/// Constructor.
	CynVirtualView::CynVirtualView(int X, int Y, int W, int H, const char *L) : Fl_Group(X, Y, W, H, L) {
		_vscroll = new V_PageScrollbar(X+W-16, Y, 16, H-16);		// will be resized by draw()
		_vscroll->type(FL_VERTICAL);
		_vscroll->maximum(100);
//		_vscroll->linesize(1);
		_hscroll = new Fl_Scrollbar(X, Y+H-16, W-16, 16);		// will be resized by draw()
		_hscroll->type(FL_HORIZONTAL);
		_hscroll->maximum(100);
		_vscroll->callback(Scrollbar_CB);
		Fl::lock();
		exchange_data.keep_running = 1; /* set this zero to expire all the child threads */
		exchange_data.widget = this;
		thread = n_create_thread(thread_func, (void *)(&exchange_data));
	}

	/// Destructor.
	CynVirtualView::~CynVirtualView() {
		delete _vscroll;
		delete _hscroll;
		Fl::unlock();
		exchange_data.keep_running = 0; /* make any pending threads expire */;
		n_wait_end_thread(thread);
	}

	int CynVirtualView::handle(int event)
	{
		int ret = Fl_Group::handle(event);
		return 1;
	}

	void CynVirtualView::draw()
	{
		std::string s = std::to_string((long long)exchange_data.count);
		fl_rectf(x(), y(), w(), 16, 255, 255, 255);
		fl_color(0, 0, 0);//font color
		fl_draw(s.c_str(), s.size(), x() + 5, y() + 12);
		_vscroll->resize(x() + w() - 16, y(), 16, h() - 16);
		draw_child(*_vscroll);
		_hscroll->resize(x(), y()+h()-16, w()-16, 16);
		draw_child(*_hscroll);
	}
}

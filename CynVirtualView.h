#ifndef CYNVIRTUALVIEW_H
#define CYNVIRTUALVIEW_H

#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Scrollbar.H>
#include <V_PageScrollbar.H>
#include <N_StringList.h>
#include <N_Threads.h>
#include <N_File_Stream.h>

using namespace ab;

namespace afltk {
	struct CVV_Thread_Data
	{
		int keep_running;
		bool painted;
		Fl_Widget *widget;
		char *buf;
		int buf_size;
		int coding;
		T_List<char*> *lines;
	};
	const int init_buf_size = 4 * 1024;
	const unsigned char BOM_UTF8_DATA[3] = { 0xEF, 0xBB, 0xBF };
	class FL_EXPORT CynVirtualView : public Fl_Group {
	protected:
		V_PageScrollbar *_vscroll;			///< Vertical scrollbar
		Fl_Scrollbar *_hscroll;			///< Horizontal scrollbar
		Fl_Thread thread;
		CVV_Thread_Data exchange_data;
		N_File_Stream *stream;
		char *buf;
		int buf_size;
		int Bom_type;
		int coding;
		T_List<char*> *lines;
	public:
		CynVirtualView(int X, int Y, int W, int H, const char *L = 0);
		~CynVirtualView();
		int handle(int event);
		void draw();
		void determineCoding();
		void setFile(const wchar_t *fileName);
		void initThread();
	};
}

#endif /*CYNVIRTUALVIEW_H*/

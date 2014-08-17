#ifndef CYNVIRTUALVIEW_H
#define CYNVIRTUALVIEW_H

#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Scrollbar.H>
#include <V_PageScrollbar.H>
#include <N_StringList.h>
#include <N_Threads.h>
#include <N_File_Stream.h>
#include <N_Mapping.h>

using namespace ab;

namespace afltk {
	const unsigned char BOM_UTF8_DATA[3] = { 0xEF, 0xBB, 0xBF };
	const int MaxLineLen = 10;
	class FL_EXPORT CynVirtualView : public Fl_Group {
		friend void Scrollbar_CB(Fl_Widget* w, void *p);
		friend void Scrollbar_CB1(Fl_Widget* w, void *p, VPS_Increment* inc);
	protected:
		V_PageScrollbar *_vscroll;			///< Vertical scrollbar
		Fl_Scrollbar *_hscroll;			///< Horizontal scrollbar
		N_Mapping *mapObj;
		char *map;
		int request_mapsize;
		int current_mapsize;
		long long mappos;
		int startmappos;
		int Bom_type;
		int coding;
		T_List<char*> *lines;
		long long filePos;
		int lineFilePos;
		int numVisibleLines;
		void init_map(const wchar_t *fileName);
		void update_map();
		int findFirstVisibleLine();
		inline int getNumVisibleLines(){ return max(0, (h() - 16) / 16); }
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

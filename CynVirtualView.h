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
#include <V_Drawer.h>
#include <Lexer.h>
#include <FileMinus.h>

using namespace ab;

namespace afltk {
	const unsigned char BOM_UTF8_DATA[3] = { 0xEF, 0xBB, 0xBF };
	const int MaxLineLen = 1024;

	class FL_EXPORT CynVirtualView : public Fl_Group {
		friend void VScrollbar_CB(Fl_Widget* w, void *p);
		friend void VScrollbar_CB1(Fl_Widget* w, void *p, VPS_Increment* inc);
		friend void HScrollbar_CB(Fl_Widget* w, void *p);
	protected:
		V_PageScrollbar *_vscroll;			///< Vertical scrollbar
		Fl_Scrollbar *_hscroll;			///< Horizontal scrollbar
		N_Mapping *mapObj;
		char *map;
		int request_mapsize;
		int current_mapsize;
		long long mappos;
		int startMappos;
		int Bom_type;
		int coding;
		T_List<char*> *lines;
		long long filePos;
		int numVisibleLines;
		void init_map(const wchar_t *fileName);
		void update_map();
		int findFirstConsiderLine(bool &lineBound);
		inline int getNumVisibleLines(){ return max(0, (h() - 16) / 16); }
		int h_changeslider; //previous h(), chenge slider only if h() changes
		uchar tabWidth;
		bool tabAlign;
		int horizPos_;
		V_Drawer *drawer;
		Lexer *lex; ///can be set outside this widget
		FileMinus *fileMinus;
	public:
		CynVirtualView(int X, int Y, int W, int H, const char *L = 0);
		~CynVirtualView();
		int handle(int event);
		void draw();
		void determineCoding();
		void setFile(const wchar_t *fileName);
		void setLex(Lexer *lex, Colorizer *colorizer);
		inline int horizPos(){return horizPos_;}
		void horizPos(int value){horizPos_ = value; redraw();}
	};
}

#endif /*CYNVIRTUALVIEW_H*/

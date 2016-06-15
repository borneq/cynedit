#ifndef CYNVIEW_H
#define CYNVIEW_H

#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <N_StringList.h>
#include <Fold.h>
#include <V_Drawer.h>
#include <Lexer.h>
#include <V_PageScrollbar.H>

using namespace ab;

namespace afltk {
class FL_EXPORT CynView : public Fl_Group {
private:
	void drawFolding(int foldIndex, int &currLine, int &pos_y);
	friend void CynView_VScrollbar_CB(Fl_Widget* w, void *p);
	friend void CynView_HScrollbar_CB(Fl_Widget* w, void *p);
protected:
	V_PageScrollbar *_vscroll;			///< Vertical scrollbar
	V_PageScrollbar *_hscroll;			///< Horizontal scrollbar
	Fold fold;
	V_Drawer *drawer;
	int horizPos_;
	uchar tabWidth;
	bool tabAlign;
	Lexer *lex; ///can be set outside this widget
public:
	int topLine; ///start display from this line
	N_StringList lines;
	CynView(int X, int Y, int W, int H, const char *L=0);
	~CynView();
	int handle(int event);
	void draw();
	inline int horizPos(){return horizPos_;}
	void horizPos(int value){horizPos_ = value; redraw();}
	void loadFromFile(wchar_t *fileName);
	void loadFromStream(N_Stream &stream);
	void setLex(Lexer *lex, Colorizer *colorizer);
};
}

#endif /*CYNVIEW_H*/

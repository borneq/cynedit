#ifndef CYNVIEW_H
#define CYNVIEW_H

#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <N_StringList.h>
#include <Fold.h>

using namespace ab;

namespace afltk {
class FL_EXPORT CynView : public Fl_Group {
private:
	void drawFolding(int foldIndex, int &currLine, int &pos_y);
protected:
	Fold fold;
public:
	int topLine; ///start display from this line
	N_StringList lines;
	CynView(int X, int Y, int W, int H, const char *L=0);
	~CynView();
	int handle(int event);
	void draw();
};
}

#endif /*CYNVIEW_H*/

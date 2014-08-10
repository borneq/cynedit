#include <FL/fl_draw.H>
#include <CynView.h>

namespace afltk {
/// Constructor.
CynView::CynView(int X, int Y, int W, int H, const char *L) : Fl_Group(X,Y,W,H,L) {
}

/// Destructor.
CynView::~CynView() {
}

int CynView::handle(int event) 
{
	return 0;
}

void CynView::draw()
{
	fl_font(FL_COURIER, 12);
	int pos_x = x();
	int pos_y = y();
	for (int i=0; i<lines.size()-1; i++)
	{
		fl_draw(lines[i], strlen(lines[i]), pos_x+5, pos_y+12);
		pos_y += 16;
	}
}
}
#include <FL/fl_draw.H>
#include <CynView.h>

namespace afltk {
/// Constructor.
CynView::CynView(int X, int Y, int W, int H, const char *L) : Fl_Group(X,Y,W,H,L) {
	topLine = 0;
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
	fl_rect(x(), y(), w(), h(), FL_BLACK);
	for (int i=topLine; i<lines.size()-1; i++)
	{
		fl_push_clip(x(),pos_y,w(), min(16, y()+h()-pos_y));
		fl_draw(lines[i], strlen(lines[i]), pos_x+5, pos_y+12);
		fl_pop_clip();
		pos_y += 16;
		if (pos_y >= y()+h()) break;
	}
}
}
#include <FL/fl_draw.H>
#include <CynView.h>

namespace afltk {
/// Constructor.
CynView::CynView(int X, int Y, int W, int H, const char *L) : Fl_Group(X,Y,W,H,L) {
	topLine = 0;
	fold.addPair(5,7);
	fold.addPair(27,16);
	fold.addPair(47,20);
	fold.addPair(69,10);
	fold.addPair(81,15);
}

/// Destructor.
CynView::~CynView() {
}

int CynView::handle(int event)
{
	int ret = Fl_Group::handle(event);
	return ret;
}

void CynView::drawFolding(int foldIndex, int &currLine, int &pos_y)
{
	while (foldIndex<fold.size())
	{
		while (currLine<=fold[foldIndex].startPos)
		{
			//fl_push_clip(x(),pos_y,w(), min(16, y()+h()-pos_y));
			fl_rectf(x(),pos_y, w(), 16, 255, 255, 255);
			fl_color(0,0,0);//font color
			fl_draw(lines[currLine], strlen(lines[currLine]), x()+5, pos_y+12);
			//fl_pop_clip();
			pos_y += 16;
			if (pos_y >= y()+h()) return;
			currLine++;
		}
		currLine += fold[foldIndex].hiddenCnt;
		foldIndex++;
	}
}

void CynView::draw()
{
	int foldIndex;
	fold.findPos(topLine, foldIndex);
	fl_font(FL_COURIER, 12);
	int pos_y = y();
	int currLine = topLine;
	drawFolding(foldIndex, currLine, pos_y);
	fl_rectf(x(),pos_y, w(), y()+h()-pos_y, 255, 255, 255);//draw remaining area
	fl_rect(x(), y(), w(), h(), FL_BLACK);
}
}

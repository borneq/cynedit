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
}
}
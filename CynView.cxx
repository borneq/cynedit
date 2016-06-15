#include <FL/fl_draw.H>
#include <N_Utf.h>
#include <CynView.h>

namespace afltk {
	void CynView_VScrollbar_CB(Fl_Widget* w, void *p)
	{
		CynView* view = (CynView*)p;
		V_PageScrollbar* scroll = (V_PageScrollbar*)w;
		view->topLine = scroll->value();
		view->redraw();
	}

	void CynView_HScrollbar_CB(Fl_Widget* w, void *p)
	{
		CynView* view = (CynView*)p;
		V_PageScrollbar* scroll = (V_PageScrollbar*)w;
		view->horizPos(scroll->value());
	}

/// Constructor.
CynView::CynView(int X, int Y, int W, int H, const char *L) : Fl_Group(X,Y,W,H,L) {
	_vscroll = new V_PageScrollbar(X+W-16, Y, 16, H-16);		// will be resized by draw()
	_vscroll->type(FL_VERTICAL);
	_vscroll->maximum(100);
	_vscroll->linesize(1);
	_vscroll->pagesize(20);
	_vscroll->callback(CynView_VScrollbar_CB, (void*)this);
	_hscroll = new V_PageScrollbar(X, Y+H-16, W-16, 16);		// will be resized by draw()
	_hscroll->type(FL_HORIZONTAL);
	_hscroll->linesize(1);
	_hscroll->maximum(100);
	_hscroll->callback(CynView_HScrollbar_CB, (void*)this);
	topLine = 0;
	horizPos_ = 0;
	drawer = new V_Drawer();
	tabWidth = 4;
	tabAlign = true;
	lex = NULL;
/*	fold.addPair(5,7);
	fold.addPair(27,16);
	fold.addPair(47,20);
	fold.addPair(69,10);
	fold.addPair(81,15);*/
}

/// Destructor.
CynView::~CynView() {
	delete drawer;
	delete _vscroll;
	delete _hscroll;
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
	int posY = y();
	//int currLine = topLine;
	//drawFolding(foldIndex, currLine, pos_y);
	int ymax = y()+h() - 16; //-16 for _hscroll height afer _hscroll resize
	drawer->init_visible_line(w()-16, horizPos_);
	for (int i = topLine; i < lines.size(); i++)
	{
		int lineLen = strlen(lines.at(i));
		//add char 10 at end
		int *ucs4line = (int *)malloc((lineLen+1)*sizeof(int));
		int ucs4len = utf8to32(lines.at(i), lineLen, ucs4line);
		ucs4line[ucs4len] = 10;
		ucs4len++; //at least 1 char
		unsigned short *lineStyle = (unsigned short *)malloc(ucs4len*sizeof(unsigned short));
		if (lex!=NULL)
		{
			lex->setLine(ucs4line,ucs4len,lineStyle);
			lex->attach();
		}
		drawer->fixedTabExpand(ucs4line, ucs4len, lineStyle, tabWidth, tabAlign);
		drawer->draw_styled_ucs4(x(), y() + (i-topLine) * 16);
		free(lineStyle);
		free(ucs4line);
		posY += 16;
		if (posY >= ymax) break;
	}
	drawer->free_visible_line();
	fl_rectf(x(),posY, w(), y()+h()-posY, 255, 255, 255);//draw remaining area

	_vscroll->resize(x() + w() - 16, y(), 16, h() - 16);
	draw_child(*_vscroll);
	_hscroll->resize(x(), y()+h()-16, w()-16, 16);
	draw_child(*_hscroll);
}

void CynView::loadFromFile(wchar_t *fileName)
{
	lines.loadFromFile(fileName);
	_vscroll->maximum(lines.size()-1);
	redraw();
}

void CynView::loadFromStream(N_Stream &stream)
{
	lines.loadFromStream(stream);
	_vscroll->maximum(lines.size()-1);
	redraw();
}

void CynView::setLex(Lexer *lex, Colorizer *colorizer)
{
	this->lex = lex;
	drawer->colorizer = colorizer;
}
}

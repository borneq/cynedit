#include <string>
#include <FL/fl_draw.H>
#include <CynVirtualView.h>
#include <N_Threads.h>
#include <N_TextUtil.h>
#include <N_TextUtil.h>

using namespace ab;

namespace afltk {
	void VScrollbar_CB(Fl_Widget* w, void *p)
	{
		CynVirtualView* view = (CynVirtualView*)p;
		V_PageScrollbar* scroll = (V_PageScrollbar*)w;
		view->filePos = (long long)((double)scroll->value() / (scroll->maximum() - scroll->minimum())*view->fileMinus->getPosFromEnd(view->numVisibleLines));
		view->redraw();
	}

	void VScrollbar_CB1(Fl_Widget* w, void *p, VPS_Increment* inc)
	{
		//CynVirtualView* view = (CynVirtualView*)p;
		//printf("cb\n");
		//Scrollbar_CB(w, p);
	}

	void HScrollbar_CB(Fl_Widget* w, void *p)
	{
		CynVirtualView* view = (CynVirtualView*)p;
		V_PageScrollbar* scroll = (V_PageScrollbar*)w;
		view->horizPos(scroll->value());
	}

	/// Constructor.
	CynVirtualView::CynVirtualView(int X, int Y, int W, int H, const char *L) : Fl_Group(X, Y, W, H, L) {
		_vscroll = new V_PageScrollbar(X+W-16, Y, 16, H-16);		// will be resized by draw()
		_vscroll->type(FL_VERTICAL);
		_vscroll->maximum(100);
		_vscroll->linesize(1);
		_vscroll->pagesize(20);
		_vscroll->callback(VScrollbar_CB, (void*)this);
		_vscroll->callbackScroll(VScrollbar_CB1, (void*)this);
		_hscroll = new Fl_Scrollbar(X, Y+H-16, W-16, 16);		// will be resized by draw()
		_hscroll->type(FL_HORIZONTAL);
		_hscroll->linesize(1);
		_hscroll->maximum(100);
		_hscroll->callback(HScrollbar_CB, (void*)this);

		Bom_type = NO_BOM;
		coding = 0;
		lines = new T_List<char*>;
		numVisibleLines = getNumVisibleLines();
		mapObj = NULL;
		h_changeslider = 0;
		tabWidth = 4;
		tabAlign = true;
		horizPos_ = 0;
		drawer = new V_Drawer();
		lex = NULL;
		fileMinus = NULL;
	}

	/// Destructor.
	CynVirtualView::~CynVirtualView() {
		delete mapObj;
		delete lines;
		delete _vscroll;
		delete _hscroll;
		delete drawer;
		delete fileMinus;
	}

	int CynVirtualView::handle(int event)
	{
		int ret = Fl_Group::handle(event);
		return 1;
	}

	//back to first line, if long lines exceeds MaxLineLen back more
	int CynVirtualView::findFirstConsiderLine(bool &lineBound)
	{
		int i = startMappos;
		backToBeginLines(map, i, numVisibleLines+1, (numVisibleLines+1)*(MaxLineLen+2), lineBound);
		if (mappos==0 && Bom_type==BOM_UTF8 && i<sizeof(BOM_UTF8_DATA))
			i=sizeof(BOM_UTF8_DATA);
		return i;
	}

	void CynVirtualView::draw()
	{
		fl_font(FL_COURIER, 12);
		numVisibleLines = getNumVisibleLines();
		int BomBytes;
		if (Bom_type==BOM_UTF8) BomBytes=sizeof(BOM_UTF8_DATA);
		else BomBytes=0;
		fileMinus->find_ending_lines(numVisibleLines, mapObj, MaxLineLen, BomBytes, coding);
		update_map();
		bool lineBound;
		int pos = findFirstConsiderLine(lineBound);
		int pos0 = pos;
		int ymax = y()+h() - 16; //-16 for _hscroll height afer _hscroll resize
		char *line;
		int blockLine = 0;
		int endType;
		int posY = y();
		int firstVisible = -1;
		while (posY < ymax && getNextLine(map, current_mapsize, line, pos, endType, MaxLineLen, coding==CODING_UTF8))
		{
			if (endType!=NO_LINE_END) lineBound = true;
			if (pos>startMappos)
			{
				if (firstVisible==-1) firstVisible=lines->size();
				posY += 16;
			}
			lines->add(line);
		}
		drawer->init_visible_line(w()-16, horizPos_);
		for (int i = 0; i < lines->size(); i++)
		{
			int lineLen = strlen(lines->at(i));
			//add char 10 at end
			int *ucs4line = (int *)malloc((lineLen+1)*sizeof(int));
			int ucs4len = utf8to32(lines->at(i), lineLen, ucs4line);
			ucs4line[ucs4len] = 10;
			ucs4len++; //at least 1 char
			unsigned short *lineStyle = (unsigned short *)malloc(ucs4len*sizeof(unsigned short));
			if (lex!=NULL)
			{
				lex->setLine(ucs4line,ucs4len,lineStyle);
				lex->attach();
			}
			if (i>=firstVisible)
			{
				drawer->fixedTabExpand(ucs4line, ucs4len, lineStyle, tabWidth, tabAlign);
				drawer->draw_styled_ucs4(x(), y() + (i-firstVisible) * 16);
			}
			free(lineStyle);
			free(ucs4line);
		}
		drawer->free_visible_line();
		if (h()!=h_changeslider)
		{
			int pp0;
			if (Bom_type==BOM_UTF8 && pos0<=sizeof(BOM_UTF8_DATA))
				pp0 = 0;
			else
				pp0 = pos0;
		  _vscroll->slider_size( (double)(pos-pp0) / mapObj->filesize() );
		  h_changeslider = h();
		}
		for (int i = 0; i < lines->size(); i++)
			free(lines->at(i));
		lines->clear();
		fl_rectf(x(),posY, w(), y()+h()-16-posY, 255, 255, 255);//draw remaining area

     	_vscroll->resize(x() + w() - 16, y(), 16, h() - 16);
		draw_child(*_vscroll);
		_hscroll->resize(x(), y()+h()-16, w()-16, 16);
		draw_child(*_hscroll);
	}

	void CynVirtualView::setFile(const wchar_t *fileName)
	{
		init_map(fileName);
		filePos = 0;
		_vscroll->value(0);
		delete fileMinus;
		fileMinus = new FileMinus();
		int BomBytes;
		if (Bom_type==BOM_UTF8) BomBytes=sizeof(BOM_UTF8_DATA);
		else BomBytes=0;
		fileMinus->find_ending_lines(numVisibleLines, mapObj, MaxLineLen, BomBytes, coding);
	}

	void CynVirtualView::setLex(Lexer *lex, Colorizer *colorizer)
	{
		this->lex = lex;
		drawer->colorizer = colorizer;
	}

    void CynVirtualView::init_map(const wchar_t *fileName)
	{
		delete mapObj;
		request_mapsize = 256*1024;
		mapObj = new N_Mapping(fileName, request_mapsize);
		map = (char*)mapObj->map(0);
		current_mapsize = mapObj->current_mapsize();
		determineCoding();
	}

	void CynVirtualView::update_map()
	{
		if (request_mapsize>=mapObj->filesize() || filePos<=request_mapsize/2)
		{
			mappos = 0;
		}
		else
		{
			assert(request_mapsize>=4*mapObj->granul());
			assert(filePos>request_mapsize/2);
			mappos = (filePos-request_mapsize/2)/mapObj->granul()*mapObj->granul();
		}
		assert(filePos>=mappos && filePos-mappos<request_mapsize);
		startMappos = (int)(filePos-mappos);
		map = (char*)mapObj->map(mappos);
		current_mapsize = mapObj->current_mapsize();
	}

	void CynVirtualView::determineCoding()
	{
		if (current_mapsize >= sizeof(BOM_UTF8_DATA))
		{
			if (memcmp(map, BOM_UTF8_DATA, sizeof(BOM_UTF8_DATA)) == 0)
				Bom_type = BOM_UTF8;
			else
				Bom_type = NO_BOM;
		}
		else Bom_type = NO_BOM;
		if (Bom_type == BOM_UTF8)
			coding = CODING_UTF8;
		else if (isUTF8(map, current_mapsize))
			coding = CODING_UTF8;
		else
			coding = CODING_LOCALE;
	}
}

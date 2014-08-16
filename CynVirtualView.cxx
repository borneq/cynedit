#include <string>
#include <FL/fl_draw.H>
#include <CynVirtualView.h>
#include <N_Threads.h>
#include <N_TextUtil.h>

using namespace ab;

namespace afltk {
	void thread_func(void* p)
	{
		CVV_Thread_Data *data = (CVV_Thread_Data *)p;
		while (data->keep_running)
		{
			/*Fl::lock(); // acquire fltk GUI lock
			data->vscroll->slider_size((double)pos / data->buf_size);
			data->widget->redraw();
			Fl::unlock(); // release fltk lock
			Fl::awake();
			while (data->keep_running && data->paintState != AFTER_PAINT)
			{
				PAUSE(30);
			}
			for (int i = 0; i < data->lines->size(); i++)
				free(data->lines->at(i));
			data->lines->clear();
			while (data->keep_running && data->paintState != NEED_THREAD_JOB)
			{
				PAUSE(30);
			}*/
		}
	}

	void Scrollbar_CB(Fl_Widget* w, void *p)
	{
		CynVirtualView* view = (CynVirtualView*)p;
		V_PageScrollbar* scroll = (V_PageScrollbar*)w;
		view->filePos = (long long)((double)scroll->value() / (scroll->maximum() - scroll->minimum())*view->stream->get_size());
		view->lineFilePos = (int)(view->numVisibleLines*(double)scroll->value() / (scroll->maximum() - scroll->minimum()));
		view->redraw();
	}

	void Scrollbar_CB1(Fl_Widget* w, void *p, VPS_Increment* inc)
	{
		//CynVirtualView* view = (CynVirtualView*)p;
		//printf("cb\n");
		//Scrollbar_CB(w, p);
	}

	/// Constructor.
	CynVirtualView::CynVirtualView(int X, int Y, int W, int H, const char *L) : Fl_Group(X, Y, W, H, L) {
		_vscroll = new V_PageScrollbar(X+W-16, Y, 16, H-16);		// will be resized by draw()
		_vscroll->type(FL_VERTICAL);
		_vscroll->maximum(100);
		_vscroll->linesize(1);
		_vscroll->pagesize(20);
		_vscroll->callback(Scrollbar_CB, (void*)this);
		_vscroll->callbackScroll(Scrollbar_CB1, (void*)this);
		_hscroll = new Fl_Scrollbar(X, Y+H-16, W-16, 16);		// will be resized by draw()
		_hscroll->type(FL_HORIZONTAL);
		_hscroll->maximum(100);

		stream = NULL;
		buf = NULL;
		buf_size = 0;
		Bom_type = NO_BOM;
		coding = 0;
		lines = new T_List<char*>;
		numVisibleLines = getNumVisibleLines();
		estimatedLineSize = 100;
	}

	/// Destructor.
	CynVirtualView::~CynVirtualView() {
		Fl::unlock();
		exchange_data.keep_running = 0; /* make any pending threads expire */;
		n_wait_end_thread(thread);
		delete stream;
		delete lines;
		free(buf);
		delete _vscroll;
		delete _hscroll;
	}

	int CynVirtualView::handle(int event)
	{
		int ret = Fl_Group::handle(event);
		return 1;
	}

	int CynVirtualView::findFirstVisibleLine()
	{
		int i = max(deltaFilePos, 0); //deltaFilePos can be <0 if BOM
		int line = lineFilePos;
		backToBeginLine(buf, i);
		while (line>0)
		{
			i--;
			backToBeginLine(buf, i);
			line--;
		}
		return i;
	}

	void CynVirtualView::draw()
	{
		/*if (exchange_data.paintState != NEED_PAINT)
		{
			exchange_data.ymax = h() - _hscroll->h();
			exchange_data.paintState = NEED_THREAD_JOB;//when resize
			return;
		}*/
		numVisibleLines = getNumVisibleLines();
		read_buf();
		int pos = findFirstVisibleLine();
		int pos0 = pos;
		int ymax = y()+h() - 16; //-16 for _hscroll height afer _hscroll resize
		char *line;
		int blockLine = 0;
		int endType;
		int posY = y();
		while (posY < ymax && getNextLine(buf, line, pos, endType))
		{
			lines->add(line);
			posY += 16;
		}
		fl_font(FL_COURIER, 12);
		for (int i = 0; i < lines->size(); i++)
		{
			fl_rectf(x(), y()+i*16, w()-16, 16, 255, 255, 255);
			fl_color(0, 0, 0);//font color
			fl_draw(lines->at(i), strlen(lines->at(i)), x() + 5, y() + i * 16 + 12);
		}
		for (int i = 0; i < lines->size(); i++)
			free(lines->at(i));
		lines->clear();
		fl_rectf(x(),posY, w(), y()+h()-16-posY, 255, 255, 255);//draw remaining area
		_vscroll->slider_size( (double)(pos-pos0) / stream->get_size() );
     	_vscroll->resize(x() + w() - 16, y(), 16, h() - 16);
		draw_child(*_vscroll);
		_hscroll->resize(x(), y()+h()-16, w()-16, 16);
		draw_child(*_hscroll);
	}

	void CynVirtualView::determineCoding()
	{
		if (buf_size >= sizeof(BOM_UTF8_DATA))
		{
			if (memcmp(buf, BOM_UTF8_DATA, sizeof(BOM_UTF8_DATA)) == 0)
				Bom_type = BOM_UTF8;
			else
				Bom_type = NO_BOM;
		}
		else Bom_type = NO_BOM;
		if (Bom_type == BOM_UTF8)
			coding = CODING_UTF8;
		else if (isUTF8(buf, buf_size))
			coding = CODING_UTF8;
		else
			coding = CODING_LOCALE;
	}

	void CynVirtualView::setFile(const wchar_t *fileName)
	{
		stream = new N_File_Stream(fileName, L"rb");
		filePos = 0;
		lineFilePos = 0;
		_vscroll->value(0);
		read_buf();
		determineCoding();
		//initThread();
	}

	void CynVirtualView::initThread()
	{
		Fl::lock();
		exchange_data.keep_running = 1; /* set this zero to expire all the child threads */
		exchange_data.widget = this;
		exchange_data.vscroll = _vscroll;
		exchange_data.ymax = h() - _hscroll->h();
		exchange_data.buf = buf;
		exchange_data.buf_size = buf_size;
		exchange_data.coding = coding;
		exchange_data.lines = lines;
		thread = n_create_thread(thread_func, (void *)(&exchange_data));
	}

	void CynVirtualView::read_buf()
	{
		buf_size = (int)min(estimatedLineSize*(numVisibleLines+1), stream->get_size());
		free(buf);
		buf = (char*)malloc(buf_size + 1);
		int start;
		if (Bom_type == BOM_UTF8)
			start = sizeof(BOM_UTF8_DATA);
		else
			start = 0;
		deltaFilePos = (int)min(estimatedLineSize*(lineFilePos+1), filePos - start);
		stream->set_position(filePos - deltaFilePos);
		int numread = stream->read(buf, buf_size);
		buf[numread] = 0;
	}
}

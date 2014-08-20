#include <Fl/fl_draw.H>
#include <N_Utf.h>
#include <V_Drawer.h>

using namespace ab;
namespace afltk{
	V_Drawer::V_Drawer()
	{
		fl_font(FL_COURIER, 12);
		fixedCharWidth=fl_width("0123456789",10)/10;
		visible_line_ = NULL;
		nVisibleFixedChars = 0;
	}

	void V_Drawer::init_visible_line(int visibleWidth, int horizPos)
	{
		nVisibleFixedChars = (int)(visibleWidth/fixedCharWidth) +1; //+1 partially visible
		horizPos_ = horizPos;
		free(visible_line_);
		visible_line_ = (int*)malloc(nVisibleFixedChars*sizeof(int));
	}
	void V_Drawer::free_visible_line()
	{
		free(visible_line_);
		visible_line_ = NULL;
	}

	//tab expanding for fixed font
	//copy maximal nVisibleFixedChars utf8 chars from in to out, one utf8 char can contains one or more chars
	void V_Drawer::fixedTabExpand(int *in, int inLen, uchar tabWidth, bool tabAlign)
	{
		visible_line_len = 0;
		int inPos = 0;
		int *out = visible_line_;
		while (visible_line_len<horizPos_ && inPos<inLen)
		{
			if (*in==9)
			{
				if (tabAlign)
				{
					do{
						visible_line_len++;
					} while(visible_line_len % tabWidth != 0 && visible_line_len<horizPos_);
					while(visible_line_len % tabWidth != 0 && visible_line_len<nVisibleFixedChars+horizPos_)
					{
						*out = 32;
						out++;
						visible_line_len++;
					}
				}
				else
				{
					int tw = tabWidth;
					for (int i=0;i<tabWidth && visible_line_len<horizPos_;i++)
					{
						visible_line_len++;
						tw--;
					}
					for (int i=0;i<tw && visible_line_len<nVisibleFixedChars+horizPos_;i++)
					{
						*out = 32;
						out++;
						visible_line_len++;
					}
				}
			}
			else
			{
				visible_line_len++;
			}
			in++;
			inPos++;
		}
		visible_line_len -= horizPos_;

		while (visible_line_len<nVisibleFixedChars && inPos<inLen)
		{
			if (*in==9)
			{
				if (tabAlign)
				do{
					*out = 32;
					out++;
					visible_line_len++;
				} while((visible_line_len+horizPos_) % tabWidth != 0 && visible_line_len<nVisibleFixedChars);
				else
				for (int i=0;i<tabWidth && visible_line_len<nVisibleFixedChars;i++)
				{
					*out = 32;
					out++;
					visible_line_len++;
				}
			}
			else
			{
				*out = *in;
				out++;
				visible_line_len++;
			}
			in++;
			inPos++;
		}
	}


	void V_Drawer::draw_ucs4(int x, int y)
	{
		if (visible_line_len<=0) return;
		char *utf8buf = (char *)malloc(4*visible_line_len+1);
		int utf8len = utf32to8(visible_line_, visible_line_len, utf8buf);
		utf8buf[utf8len] = 0;
		fl_draw(utf8buf, utf8len, x, y);
		free(utf8buf);
	}
	void V_Drawer::draw_styled_ucs4(int* str, int strlen, int *style, int x, int y)
	{
		char *utf8buf = (char *)malloc(4*strlen+1);
		int utf8len = utf32to8(str, strlen, utf8buf);
		utf8buf[utf8len] = 0;
		fl_draw(utf8buf, utf8len, x, y);
		free(utf8buf);
	}

}

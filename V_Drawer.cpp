#include <Fl/fl_draw.H>
#include <N_Utf.h>
#include <V_Drawer.h>
#include <math.h>

using namespace ab;
namespace afltk{
	V_Drawer::V_Drawer()
	{
		fl_font(FL_COURIER, 12);
		fixedCharWidth=fl_width("0123456789",10)/10;
		visible_line_ = NULL;
		visible_style_ = NULL;
		nVisibleFixedChars = 0;
		colorizer = NULL;
	}

	void V_Drawer::init_visible_line(int visibleWidth, int horizPos)
	{
		visibleWidth_ = visibleWidth;
		nVisibleFixedChars = (int)(visibleWidth/fixedCharWidth) +1; //+1 partially visible
		horizPos_ = horizPos;
		free(visible_line_);
		free(visible_style_);
		visible_line_ = (int*)malloc(nVisibleFixedChars*sizeof(int));
		visible_style_ = (unsigned short*)malloc(nVisibleFixedChars*sizeof(unsigned short));
	}

	void V_Drawer::free_visible_line()
	{
		free(visible_line_);
		visible_line_ = NULL;
		free(visible_style_);
		visible_style_ = NULL;
	}

	//tab expanding for fixed font
	//copy maximal nVisibleFixedChars utf8 chars from in to out, one utf8 char can contains one or more chars
	void V_Drawer::fixedTabExpand(int *in, int inLen, unsigned short *style_in, uchar tabWidth, bool tabAlign)
	{
		visible_line_len = 0;
		int inPos = 0;
		int *out = visible_line_;
		unsigned short *style_out = visible_style_;

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
						*style_out = *style_in;
						out++;
						style_out++;
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
						*style_out = *style_in;
						out++;
						style_out++;
						visible_line_len++;
					}
				}
			}
			else
			{
				visible_line_len++;
			}
			in++;
			style_in++;
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
					*style_out = *style_in;
					out++;
					style_out++;
					visible_line_len++;
				} while((visible_line_len+horizPos_) % tabWidth != 0 && visible_line_len<nVisibleFixedChars);
				else
				for (int i=0;i<tabWidth && visible_line_len<nVisibleFixedChars;i++)
				{
					*out = 32;
					*style_out = *style_in;
					out++;
					style_out++;
					visible_line_len++;
				}
			}
			else
			{
				*out = *in;
				*style_out = *style_in;
				out++;
				style_out++;
				visible_line_len++;
			}
			in++;
			style_in++;
			inPos++;
		}
	}


	void V_Drawer::draw_styled_ucs4(int x, int y)
	{
		if (visible_line_len<=0)
		{
			fl_rectf(x, y, visibleWidth_, 16, 255, 255, 255);
			return;
		}
		char *utf8buf = (char *)malloc(4*visible_line_len+1);//UTF8 can have 4 bytes/char
		unsigned short style;
		int pos = 0;
		while (pos<visible_line_len)
		{
			style = visible_style_[pos];
			int pos0=pos;
			while (pos<visible_line_len && style == visible_style_[pos]) pos++;
			int utf8len = utf32to8(visible_line_+pos0, pos-pos0, utf8buf);
			utf8buf[utf8len] = 0;
			int xpos = (int)floor(x+fixedCharWidth*pos0);
			int lenchars = (int)ceil(fixedCharWidth*(pos-pos0));
			Color bgColor, fontColor;
			if (colorizer!=NULL)
			{
				colorizer->getProp(style, bgColor, fontColor);
				fl_rectf(xpos, y, lenchars, 16, bgColor.r, bgColor.g, bgColor.b);
				fl_color(fontColor.r, fontColor.g, fontColor.b);
			}
			else
			{
				fl_rectf(xpos, y, lenchars, 16, 255, 255, 255);
				fl_color(FL_BLACK);
			}
			fl_draw(utf8buf, utf8len, xpos, y+12);
		}
		free(utf8buf);
		int xpos = (int)ceil(x+fixedCharWidth*pos);
		fl_rectf(xpos, y, x+visibleWidth_-xpos, 16, 255, 255, 255);
	}
}

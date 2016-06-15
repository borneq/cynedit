#ifndef V_Drawer_H
#define V_Drawer_H

#include <Colorizer.h>

using namespace ab;

namespace afltk{
class V_Drawer
{
protected:
	int visibleWidth_;
	double fixedCharWidth;
	int horizPos_;
	int *visible_line_;
	unsigned short *visible_style_;
	int visible_line_len;
	int nVisibleFixedChars;
public:
	V_Drawer();
	Colorizer *colorizer;
	void init_visible_line(int visibleWidth, int horizPos);
	void free_visible_line();
	void fixedTabExpand(int *in, int inLen, unsigned short *style_in, uchar tabWidth, bool tabAlign);
	void draw_styled_ucs4(int x, int y);
};
}
#endif /*V_Drawer_H*/

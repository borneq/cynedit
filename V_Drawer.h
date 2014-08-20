#ifndef V_Drawer_H
#define V_Drawer_H

namespace afltk{
class V_Drawer
{
protected:
	double fixedCharWidth;
	int horizPos_;
	int *visible_line_;
	int visible_line_len;
	int nVisibleFixedChars;
public:
	V_Drawer();
	void init_visible_line(int visibleWidth, int horizPos);
	void free_visible_line();
	void fixedTabExpand(int *in, int inLen, uchar tabWidth, bool tabAlign);
	void draw_ucs4(int x, int y);
	void draw_styled_ucs4(int* str, int strlen, int *style, int x, int y);
};
}
#endif /*V_Drawer_H*/

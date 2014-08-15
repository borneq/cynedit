/* \file
V_PageScrollbar widget . */

#ifndef V_PageScrollbar_H
#define V_PageScrollbar_H

#include "FL/Fl_Slider.H"

namespace afltk {
	class FL_EXPORT V_PageScrollbar : public Fl_Slider {
	public:
		V_PageScrollbar(int X, int Y, int W, int H, const char *L = 0);
		//~V_PageScrollbar();
	};
}

#endif /*V_PageScrollbar_H*/
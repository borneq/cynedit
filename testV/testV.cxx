#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <CynView.h>

using namespace afltk;

void quit_cb(Fl_Widget*, void*) {exit(0);}

Fl_Menu_Item menutable[] = {
  {"&File",0,0,0,FL_SUBMENU},
    {"E&xit",	FL_ALT+FL_F+4, quit_cb, 0, FL_MENU_DIVIDER},
	{0},
};

Fl_Box* create_status(Fl_Window &window)
{
  Fl_Box *statusbar = new Fl_Box(0,window.h()-20,window.w(),20);
  statusbar->box(FL_FLAT_BOX);
  statusbar->color(46);
  statusbar->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  statusbar->copy_label("msg");
  return statusbar;
}

int main(int argc, char** argv) {
  Fl_Window window(300,300);
  window.box(FL_NO_BOX);
  Fl_Box *statusbar = create_status(window);
  Fl_Menu_Bar menubar(0,0,window.w(),20);
  menubar.menu(menutable);
  CynView viewer(150,50,75,75);
  viewer.lines.loadFromFile(L"../CynView.cxx");
  window.end();
  window.show(argc,argv);
  return Fl::run();
}

#ifndef Colorizer_H
#define Colorizer_H

namespace ab{
struct Color{
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

class Colorizer
{
public:
	virtual void getProp(unsigned short style, Color &bgColor, Color &fontColor) = 0;
};
}
#endif /*Colorizer_H*/

#ifndef ColorsC_H
#define ColorsC_H

#include <Colorizer.h>

namespace ab{
class ColorsC : public Colorizer
{
#include "tokensC.h"
public:
	void getProp(unsigned short style, Color &bgColor, Color &fontColor);
};
}
#endif /*ColorsC_H*/

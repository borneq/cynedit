#include "ColorsC.h"

namespace ab{
void ColorsC::getProp(unsigned short style, Color &bgColor, Color &fontColor)
{
	switch (style)
	{
	case LEX_TYPE: bgColor.r=255;bgColor.g=200;bgColor.b=200;
				  fontColor.r=255;fontColor.g=0;fontColor.b=0;
				  break;
	case LEX_ID:  bgColor.r=255;bgColor.g=200;bgColor.b=200;
				  fontColor.r=0;fontColor.g=0;fontColor.b=255;
				  break;
	case LEX_COMMENT: bgColor.r=255;bgColor.g=255;bgColor.b=255;
				 fontColor.r=0;fontColor.g=128;fontColor.b=0;
				 break;
	case LEX_STRING_LITERAL:
				 bgColor.r=255;bgColor.g=255;bgColor.b=255;
				 fontColor.r=255;fontColor.g=0;fontColor.b=255;
				 break;
	default: bgColor.r=255;bgColor.g=255;bgColor.b=255;
			 fontColor.r=0;fontColor.g=0;fontColor.b=0;
	}
}
}

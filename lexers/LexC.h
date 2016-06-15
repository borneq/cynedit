#ifndef LexC_H
#define LexC_H

#include <Lexer.h>

namespace ab{
class LexC : public Lexer
{
#include "tokensC.h"
protected:
    bool fLETTER();
    bool fDIGIT();
    void mTYPE();
    void mWS();
    void mCOMMENT_LINE();
    void mCOMMENT_MULTI();
    void mID();
    void mSTRING_LITERAL();
    void mINT();
    void mRETURN();
public:
	void execute();
	LexC():Lexer(){};
};
}
#endif /*LexC_H*/

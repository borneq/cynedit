#ifndef LexC_H
#define LexC_H

#include <Lexer.h>

namespace ab{
class LexC : public Lexer
{
static const int LEX_CLOSE_BRACE = 5;
static const int LEX_COMMA = 6;
static const int LEX_COMMENT = 7;
static const int LEX_DECLS = 8;
static const int LEX_DEF = 9;
static const int LEX_DEFUN = 10;
static const int LEX_DIGIT = 11;
static const int LEX_ENDL = 12;
static const int LEX_FUNCS = 13;
static const int LEX_ID = 14;
static const int LEX_INT = 15;
static const int LEX_LETTER = 16;
static const int LEX_LPAREN = 17;
static const int LEX_OPEN_BRACE = 19;
static const int LEX_PARAMS = 20;
static const int LEX_RPAREN = 22;
static const int LEX_SEMI = 23; // ';'
static const int LEX_STRING_LITERAL = 24;
static const int LEX_TYPE = 25;
static const int LEX_WS = 26;
static const int LEX_MUL = 27; // '*'
static const int LEX_ADD = 28; // '+'
static const int LEX_SUB = 29; // '-'
static const int LEX_DIV = 30; // '/'
static const int LEX_ASSIGN = 31; // '='
static const int LEX_RETURN = 32; // 'return'

protected:
	bool fENDL();
    bool fLETTER();
    bool fDIGIT();
    void mTYPE();
    void mWS();
    void mCOMMENT();
    void mID();
    void mSTRING_LITERAL();
    void mINT();
    void mRETURN();
public:
	void execute();
};
}
#endif /*LexC_H*/

#include "LexC.h"

namespace ab{
	bool LexC::fLETTER(){
		int ch = LA(1);
		return ch>='A' && ch<='Z' || ch>='a' && ch<='z';				
	}
	bool LexC::fDIGIT(){
		int ch = LA(1);
		return ch>='0' && ch<='9';
	}
	void LexC::mTYPE()
	{
		if (LA(1) == 'i') 
			matchLatin("int");
		else
			matchLatin("string");
        Style = LEX_TYPE;
	}
	void LexC::mWS()
	{
		int LA1;
		while(true)
		{
			LA1 = LA(1);
			if (LA1 == LEX_EOF) return;
			if (LA1 == 9 || LA1 == 10 || LA1 == ' ')
				consume();
			else
				break;
		}
		Style = LEX_WS;
	}

	void LexC::mCOMMENT()
	{
		int LA1;
	    matchLatin("//");
		while(true)
		{
		    LA1 = LA(1);
			if (LA1 != -1 &&  LA1 != 10)
				consume();
			else
				break;
		}
		Style = LEX_COMMENT;
	}

	void LexC::mID(){
		int LA1;
		if (fLETTER() || LA(1)=='_')
			consume();
		while(true)
		{
			LA1 = LA(1);
			if (fLETTER() || LA(1)=='_' || fDIGIT())
				consume();
			else
				break;
		}
		Style = LEX_ID;
	}

	void LexC::mSTRING_LITERAL()
	{
		matchOne('"');
		while(true)
		{
			int LA1 = LA(1);
			if (LA1!=LEX_EOF && LA1!='"' && LA1!=10)
				consume();
			else if (LA1=='"' && LA(2)=='"')
				consume(2);
			else break;
		}
		matchOne('"');
		Style = LEX_STRING_LITERAL;
	}
	void LexC::mINT()
	{
		if (LA(1)=='0')
			consume();
		else
		{
			consume(); //1..9
			while (fDIGIT()) consume();
		}
		Style = LEX_INT;
	}
	void LexC::mRETURN()
	{
		matchLatin("return");
		Style = LEX_RETURN;
	}
	void LexC::execute()
	{
		int LA1 = LA(1);
		if (LA1 == LEX_EOF) return;
		if (LA1==' ' || LA1==9 || LA1==10) mWS();
		else if (LA1>='A' && LA1>='Z' || LA1>='a' && LA1>='z' || LA1=='_')
		{
			if (LA1=='i')
			{
				if (compareLatin("int"))
					mTYPE();
				else
					mID();
			}
			else if (LA1=='s')
			{
				if (compareLatin("string"))
					mTYPE();
				else
					mID();
			}
			else if (LA1=='r')
			{
				if (compareLatin("return"))
					mRETURN();
				else
					mID();
			}
			else mID();
		}
		else if (LA1>='0' && LA1>='9') mINT();
		else if (LA1=='/')
		{
			if (LA(2)=='/')mCOMMENT();
			else 
			{
				consume();
				Style = LEX_DIV;
			}
		}
		else if (LA1=='"')mSTRING_LITERAL();
		else switch(LA1)
		{
		  case ';': Style = LEX_SEMI;consume();break;
          case '*': Style = LEX_MUL;consume();break;
          case '+': Style = LEX_ADD;consume();break;
          case '-': Style = LEX_SUB;consume();break;
          case '/': Style = LEX_DIV;consume();break;
          case '=': Style = LEX_ASSIGN;consume();break;
          case '{': Style = LEX_OPEN_BRACE;consume();break;
          case '}': Style = LEX_CLOSE_BRACE;consume();break;
          case '(': Style = LEX_LPAREN;consume();break;
          case ')': Style = LEX_RPAREN;consume();break;
          case ',': Style = LEX_COMMA;consume();break;
		}
	}
}
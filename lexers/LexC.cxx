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
			approve("int",LEX_TYPE);
		else
			approve("string",LEX_TYPE);
	}
	void LexC::mWS()
	{
		int LA1;
		while(true)
		{
			LA1 = LA(1);
			if (LA1 == LEX_EOF) return;
			if (LA1 == 9 || LA1 == 10 || LA1 == ' ')
				approve(LEX_WS);
			else
				break;
		}
	}

	void LexC::mCOMMENT_LINE()
	{
		int LA1;
	    approve("//",LEX_COMMENT);
		while(true)
		{
		    LA1 = LA(1);
			if (LA1 != -1 &&  LA1 != 10)
				approve(LEX_COMMENT);
			else
				break;
		}
	}

	void LexC::mCOMMENT_MULTI()
	{
		int LA1;
	    approve("/*",LEX_COMMENT);
		while(true)
		{
			LA1 = LA(1);
			if (LA1 != '*') approve(LEX_COMMENT);
			else
			{
				if (LA(2)=='/')
				{
					approve(2, LEX_COMMENT);
					break;
				}
				else approve(2, LEX_COMMENT);
			}
    	}
	}

	void LexC::mID(){
		int LA1;
		if (fLETTER() || LA(1)=='_')
			approve(LEX_ID);
		while(true)
		{
			LA1 = LA(1);
			if (fLETTER() || LA(1)=='_' || fDIGIT())
				approve(LEX_ID);
			else
				break;
		}
	}

	void LexC::mSTRING_LITERAL()
	{
		approve(LEX_STRING_LITERAL);
		while(true)
		{
			int LA1 = LA(1);
			if (LA1=='\\' && LA(2)!=LEX_EOF && LA(2)!=10)
				approve(2,LEX_STRING_LITERAL);
			if (LA1!=LEX_EOF && LA1!='"' && LA1!=10)
				approve(LEX_STRING_LITERAL);
			else break;
		}
		approve(LEX_STRING_LITERAL);
	}
	void LexC::mINT()
	{
		if (LA(1)=='0')
			approve(LEX_INT);
		else
		{
			approve(LEX_INT); //1..9
			while (fDIGIT()) approve(LEX_INT);
		}
	}

	void LexC::mRETURN()
	{
		approve("return",LEX_RETURN);
	}

	void LexC::execute()
	{
		while (true)
		{
			int LA1 = LA(1);
			if (LA1 == LEX_EOF) return;
			if (LA1==' ' || LA1==9 || LA1==10) mWS();
			else if (LA1>='A' && LA1<='Z' || LA1>='a' && LA1<='z' || LA1=='_')
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
			else if (LA1>='0' && LA1<='9') mINT();
			else if (LA1=='/')
			{
				if (LA(2)=='/')mCOMMENT_LINE();
				else if (LA(2)=='*')mCOMMENT_MULTI();
				else
				{
					approve(LEX_DIV);
				}
			}
			else if (LA1=='"')mSTRING_LITERAL();
			else switch(LA1)
			{
			  case ';': approve(LEX_SEMI);break;
			  case '*': approve(LEX_MUL);break;
			  case '+': approve(LEX_ADD);break;
			  case '-': approve(LEX_SUB);break;
			  case '/': approve(LEX_DIV);break;
			  case '=': approve(LEX_ASSIGN);break;
			  case '{': approve(LEX_OPEN_BRACE);break;
			  case '}': approve(LEX_CLOSE_BRACE);break;
			  case '(': approve(LEX_LPAREN);break;
			  case ')': approve(LEX_RPAREN);break;
			  case ',': approve(LEX_COMMA);break;
			  default: approve(LEX_DEFAULT);
			}
		}
	}
}

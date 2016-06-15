#include <Lexer.h>

namespace ab{
	Lexer::Lexer(): N_Coroutine(64*1024)
	{
		line_ = NULL;
		lineStyle_ = NULL;
	    lineLen_ = 0;
		posLA = 0;
		posApprove = 0;
	}

	void Lexer::setLine(int *line, int lineLen, unsigned short *lineStyle)
	{
		line_ = line;
		lineStyle_ = lineStyle;
	    lineLen_ = lineLen;
		posLA = 0;
		posApprove = 0;
	}

	int Lexer::LA(int k)
	{
		int pos = posApprove+(k-1);
		assert(pos>=0 && pos<lineLen_);
		return line_[pos];
	}

	void Lexer::approve(unsigned short style)
	{
		lineStyle_[posApprove] = style;
		posApprove++;
		assert(posApprove<=lineLen_);
		if (posApprove==lineLen_) yield();
	}

	void Lexer::approve(int n, unsigned short style)
	{
		for (int i=0; i<n; i++)
			approve(style);
	}

	void Lexer::approve(char *s, unsigned short style)
	{
		approve(strlen(s), style);

	}

	bool Lexer::compareLatin(char *s)
	{
		int len = strlen(s);
		for (int i=0; i<len; i++)
		{
			if (s[i]!=LA(i+1)) return false;
		}
		return true;
	}
}

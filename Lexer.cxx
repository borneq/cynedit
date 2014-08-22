#include <Lexer.h>

namespace ab{
	int Lexer::LA(int k)
	{
		return charQueue.peek(k-1);
	}

	void Lexer::consume(int n)
	{
		for (int i=0; i<n; i++)
			charQueue.pop();
	}

	void Lexer::matchOne(char c)
	{
		consume();
	}

	void Lexer::matchLatin(char *s)
	{
		consume(strlen(s));
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

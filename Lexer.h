#ifndef Lexer_H
#define Lexer_H

#include <T_Queue.hpp>
#include <N_Coroutine.h>

namespace ab{
class Lexer : public N_Coroutine
{
private:
	int *line_;
	unsigned short *lineStyle_;
	int lineLen_;
	int posLA, posApprove;
protected:
	static const int LEX_EOF = -1;
	static const int LEX_DEFAULT = 0;
	int LA(int k);
	void approve(unsigned short Style);
	void approve(int n, unsigned short style);
	void approve(char *s, unsigned short style);
	bool compareLatin(char *s);
public:
	Lexer();
	void setLine(int *line, int lineLen, unsigned short *lineStyle);
};
}
#endif /*Lexer_H*/

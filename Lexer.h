#ifndef Lexer_H
#define Lexer_H

#include <T_Queue.hpp>
#include <N_Coroutine.h>

namespace ab{
class Lexer : public N_Coroutine
{
private:
	T_Queue<int> charQueue;
protected:
	static const int LEX_EOF = -1;
	int LA(int k);
	void consume(int n=1);
	void matchOne(char c);
	void matchLatin(char *s);
	bool compareLatin(char *s);
public:
	unsigned short Style;
};
}
#endif /*Lexer_H*/

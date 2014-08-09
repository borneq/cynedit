#include <N_Stream.h>

namespace ab {
char N_Stream::readByte()
{
	char b;
	read(&b, sizeof(b));
	return b;
}
short N_Stream::ReadWord()
{
	short w;
	read(&w, sizeof(w));
	return w;
}
long N_Stream::ReadDWord()
{
	long d;
	read(&d, sizeof(d));
	return d;
}
long long N_Stream::ReadQWord()
{
	long long q;
	read(&q, sizeof(q));
	return q;
}
void N_Stream::writeByte(char b)
{
	write(&b, sizeof(b));
}
void N_Stream::WriteWord(short w)
{
	write(&w, sizeof(w));
}
void N_Stream::WriteDWord(long d)
{
	write(&d, sizeof(d));
}
void N_Stream::WriteQWord(long long q)
{
	write(&q, sizeof(q));
}
}
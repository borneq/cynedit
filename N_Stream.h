#ifndef N_STREAM_H
#define N_STREAM_H

namespace ab {
/// Abstract class, base of streams
class N_Stream {
protected:
    long long _position;
	long long _size;
public:	
	virtual size_t read(void *buffer, size_t size) = 0;
	virtual size_t write(void *buffer, size_t size) = 0;
	char readByte();
	short ReadWord();
	long ReadDWord();
	long long ReadQWord();
    void writeByte(char b);
	void WriteWord(short w);
	void WriteDWord(long d);
	void WriteQWord(long long q);
	virtual long long get_position(){return _position;}
	virtual long long get_size(){return _size;}
};  
}

#endif /*N_STREAM_H*/
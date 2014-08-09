#ifndef N_SEEK_STREAM_H
#define N_SEEK_STREAM_H

#include <N_Stream.h>

namespace ab {
/// Abstract class, base of streams wih seek
class N_Seek_Stream : public N_Stream {
public:
	virtual bool seek(long long pos) = 0;
	virtual bool seek(long offset, int origin) = 0;
	virtual void set_position(long long position) = 0;
};
}

#endif /*N_SEEK_STREAM_H*/

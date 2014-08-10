#ifndef N_FILE_STREAM_H
#define N_FILE_STREAM_H

#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <stdexcept>
#include <n_utf.h>
#include <N_Seek_Stream.h>

namespace ab {
/// FILE wrapper
class N_File_Stream : public N_Seek_Stream {
protected:
	FILE *pFile;
public:
	N_File_Stream(const wchar_t *filename, const wchar_t *mode)
	{
		pFile = _wfopen(filename, mode);
		if (pFile==NULL) throw std::invalid_argument("N_File_Stream can't open filename");
		_size = -1;
	}
	N_File_Stream(const char *filenameUTF8, const char *mode )
	{
		wchar_t *wbuf = allocUtf16for8(filenameUTF8);
		wchar_t *wmodebuf = allocUtf16for8(mode);
		N_File_Stream(wbuf, wmodebuf);
		free(wbuf);
		free(wmodebuf);
	}

	~N_File_Stream(){fclose(pFile);}

	size_t read(void *buffer, size_t size);
	size_t write(void *buffer, size_t size);
	long long get_position();
	void set_position(long long position);
	long long get_size();
	bool seek(long long position);
	bool seek(long offset, int origin);
};
}

#endif /*N_FILE_STREAM_H*/

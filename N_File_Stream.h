#ifndef N_FILE_STREAM_H
#define N_FILE_STREAM_H

#include <stdio.h>
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
		_size = -1;
	}
	N_File_Stream(const char *filenameUTF8, const char *mode ){throw "not implemented";}

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
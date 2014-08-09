#include <stdio.h>
#include <N_File_Stream.h>

namespace ab {
size_t N_File_Stream::read(void *buffer, size_t size)
{
	return fread(buffer, size, 1, pFile);
}

size_t N_File_Stream::write(void *buffer, size_t size)
{
	return fwrite(buffer, size, 1, pFile);
}

long long N_File_Stream::get_position()
{
	long long position;	
	fgetpos(pFile, &position);
	return position;
}

void N_File_Stream::set_position(long long position)
{
	fsetpos(pFile, &position);
}
long long N_File_Stream::get_size()
{
	if (_size == -1)
	{
		long long savpos;	
		fgetpos(pFile, &savpos);
		fseek(pFile,0L,SEEK_END);
		fgetpos(pFile, &_size);
		fsetpos(pFile, &savpos);
	}
	return _size;
}

bool N_File_Stream::seek(long long position)
{
	return fsetpos(pFile, &position) == 0;
}

bool N_File_Stream::seek(long offset, int origin)
{
	return fseek(pFile,offset,origin) == 0;
}
}
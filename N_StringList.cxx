#include <algorithm>
#include <N_StringList.h>
#include <N_File_Stream.h>
#include <N_TextUtil.h>

namespace ab {

void N_StringList::clear()
{
	for (int i=0; i<size(); i++)
		free(_list[i]);
	T_List<char *>::clear();
}
void N_StringList::loadFromFile(wchar_t *fileName)
{
	N_File_Stream stream(fileName, L"r");
	loadFromStream(stream);

}
void N_StringList::loadFromStream(N_Stream &stream)
{
	const int BLOCKSIZE = 10;
	int currentBlockSize = (int)std::min((long long)BLOCKSIZE, stream.get_size());
	char *buf = (char*) malloc(currentBlockSize+1);

	while(true)
	{
		int readbytes = stream.read(buf, currentBlockSize);
		if (readbytes==0) break;
		buf[readbytes] = 0;
		int pos = 0;
		char *line;
		while (getNextLine(buf,line,pos)) add(line);
	}
	free(buf);
}
}
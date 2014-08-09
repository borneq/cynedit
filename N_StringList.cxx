#include <N_StringList.h>
#include <N_File_Stream.h>

namespace ab {
void N_StringList::loadFromFile(wchar_t *fileName)
{
	N_File_Stream stream(fileName, L"r");
	loadFromStream(stream);

}
void N_StringList::loadFromStream(N_Stream &stream)
{
	const int blockSize = 1000;
	char *buf = (char*) malloc(blockSize);
	stream.read(buf, blockSize);
	free(buf);
}
}
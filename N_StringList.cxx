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
	N_File_Stream stream(fileName, L"rb");
	loadFromStream(stream);

}
void N_StringList::loadFromStream(N_Stream &stream)
{
	int endingsCounts[4];
	for (int i=0; i<4; i++) endingsCounts[i]=0;
	const int BLOCKSIZE = 6;
	int currentBlockSize = (int)std::min((long long)BLOCKSIZE, stream.get_size());
	char *buf = (char*) malloc(currentBlockSize+1);

	char lastPriorBlockChar = 0;
	while(true)
	{
		int readbytes = stream.read(buf, currentBlockSize);
		if (readbytes==0) break;
		buf[readbytes] = 0;
		int pos = 0;
		char *line;
		int blockLine = 0;
		int endType;
		while (getNextLine(buf,line,pos,endType))
		{
			assert(endType>=0 && endType<=3);
			//if read all line but no CR,LF - next line is empty, merge OK
			//if read with LF in Linux or all CRLF in Windows - previous line is empty, next not empty, merge OK
			//problem is in Windows when read CR but not LF  -previous and next line are empty
			//must completely remove two empty lines
			if (blockLine==0 && size()>0)
			{
				if (lastPriorBlockChar==13 && buf[0]==10)
				{
					assert(_list[size()-1][0]==0 && line[0]==0);
					del(size()-1);
				}
				else
				{
					if (endType>0 && endType<4)
						endingsCounts[endType]++;
					_list[size()-1] = mergeLines(_list[size()-1], line);
				}
			}
			else
			{
				if (endType>0 && endType<4)
						endingsCounts[endType]++;
				add(line);
			}
			blockLine++;
		}
		assert(readbytes-1>=0);
		lastPriorBlockChar = buf[readbytes-1];
	}
	free(buf);
	lineEndings=0;
	int maxCnt=0;
	for (int i=0; i<4; i++)
		if (endingsCounts[i]>maxCnt)
		{
			maxCnt = endingsCounts[i];
			lineEndings = i;
		}
}
}
/*szczegolny przypadek - gdy przeczya x \n wtedy dodaje do pustej - ok
	ale gdy przeczyta do \n - wtedt nowea linia jest pusta, tez ok
gorszy przypadek, gdy na dwuznaku przerywa - wtedy ostatnia linia jest pusta, pierwsza z nastepnych jest pusta
³¹czy je i wstawia pust¹ liniê
porzebny jest znak koncowy linii i znak pocz¹tkowy nowej linii
wtedy gdy bêdzie CRLF nalezaloby j¹ pomijac
poza tym to potrzebne do gromadzenia znaków konca linii, aby zapisaæ tak jak maksymalnie bylo
albo zapisywac bez zmian , opcjonalnie
czyli nie tylok char* pamietac , ale char*, int len, enum lineendings - 9 bajtów zamiast 4
ale na razie nie, jedynie zliczac cr i lf
*/

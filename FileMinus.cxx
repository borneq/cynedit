#include <FileMinus.h>
#include <N_Utf.h>
#include <N_TextUtil.h>

namespace ab {
void FileMinus::find_ending_lines(int numVisibleLines, N_Mapping *mapObj, int MaxLineLen, int BOMbytes, int coding)
{
char *map;
int startPos,endPos;
	if (numVisibleLines<=maxNumVisible) return;
	long long mappos;
	if (mapObj->current_mapsize()<=mapObj->filesize())
	{
		mappos = 0;
	}
	else
	{
		long long mappos = (mapObj->filesize()-mapObj->request_mapsize()/2)/mapObj->granul()*mapObj->granul();
	}
	map = (char*)mapObj->map(mappos);
	startPos = (int)(mapObj->filesize()-mappos);
	endPos = startPos;
	bool lineBound;
	backToBeginLines(map, startPos, numVisibleLines+1, (numVisibleLines+1)*(MaxLineLen+2), lineBound);
	if (mappos==0 && startPos<BOMbytes)
		startPos = BOMbytes;
	int endType;
	char *line;
	list.clear();
	ElemIndex ei;
	ei.idx = startPos+mappos;
	ei.lineBound = lineBound;
	list.add(ei);
	while (getNextLine(map, endPos, line, startPos, endType, MaxLineLen, coding==CODING_UTF8))
	{
		printf("%d\n",startPos);
		if (endType!=NO_LINE_END) lineBound = true;
		free(line);
		ei.idx = startPos+mappos;
		ei.lineBound = lineBound;
		list.add(ei);
	}
	maxNumVisible = numVisibleLines;
}

long long FileMinus::getPosFromEnd(int nFromEnd)
{
	if (list.size()==0) return 0;
	int pos = list.size()-1-nFromEnd;
	if (pos<0) pos = 0;
	return list[pos].idx;
}
}

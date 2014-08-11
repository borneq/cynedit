#include <Fold.h>

namespace ab {

int compare(FoldItem &item1,FoldItem &item2)
{
	if (item1.startPos<item2.startPos) return -1;
	else if (item1.startPos>item2.startPos) return 1;
	else return 0;
}

void Fold::addPair(int startPos, int hiddenCnt)
{
	FoldItem fi;
	fi.startPos = startPos;
	fi.hiddenCnt = hiddenCnt;
	add(fi);
	sumHidden += hiddenCnt;
}

bool Fold::findPos(int pos, int &index)
{
	FoldItem item;
	item.startPos = pos;
	item.hiddenCnt = 0;
	return binaryFind(item, index);
}
}
#include <Fold.h>

namespace ab {
void Fold::addPair(int startPos, int hiddenCnt)
{
	FoldItem fi;
	fi.startPos = startPos;
	fi.hiddenCnt = hiddenCnt;
	add(fi);
	sumHidden += hiddenCnt;
}
}
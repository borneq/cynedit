#include <T_List.hpp>

namespace ab {

class FoldItem
{
public:
	int startPos;
	int hiddenCnt;
};

int compare(FoldItem &item1,FoldItem &item2);

class Fold : public T_List<FoldItem>
{
public:
	int sumHidden;
	void addPair(int startPos, int hiddenCnt);
	Fold(){sumHidden = 0; compareP = compare;}
	bool findPos(int pos, int &index);
};
}

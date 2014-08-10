#include <T_List.hpp>

namespace ab {

class FoldItem
{
public:
	int startPos;
	int hiddenCnt;
};


class Fold : public T_List<FoldItem>
{
public:
	int sumHidden;
	void addPair(int startPos, int hiddenCnt);
	Fold(){sumHidden = 0;}
};
}

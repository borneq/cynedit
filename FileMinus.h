//auxiliary class to CyynVirtualView
#ifndef FileMinus_h
#define FileMinus_h

#include <N_Mapping.h>
#include <T_List.hpp>

namespace ab {
	struct ElemIndex {
		long long idx; //begin of line
		bool lineBound;
	};

	class FileMinus {
	private:
		int maxNumVisible;
		T_List<ElemIndex> list;
	public:
		void find_ending_lines(int numVisibleLines, N_Mapping *mapObj, int MaxLineLen, int BOMbytes, int coding);
		long long getPosFromEnd(int nFromEnd);
		FileMinus()
		{
			maxNumVisible = 0;
		}
	};
}

#endif /*FileMinus_h*/

#ifndef N_STRINGLIST_H
#define N_STRINGLIST_H
#include <T_List.hpp>
#include <N_Stream.h>

namespace ab {
class N_StringList : public T_List<char *> {
public:
	void loadFromFile(wchar_t *fileName);
	void loadFromStream(N_Stream &stream);
};
}

#endif /*N_STRINGLIST_H*/
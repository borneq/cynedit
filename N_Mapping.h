#ifdef WIN32
#include <Windows.h>
#else
#endif
namespace ab {
	class N_Mapping {
	protected:
		HANDLE handle;
		HANDLE maphandle;
	    int mapsize_;
		long long filesize_;
		void *mapptr;
		unsigned long granul;
	public:
		N_Mapping(const wchar_t *filename, int mapSize);
		void* map(long long offset);
		void* map(long long offset, int newMapSize);
		long long size();
		~N_Mapping();
	};
}
#ifndef N_Mapping_h
#define N_Mapping_h

#ifdef WIN32
#include <Windows.h>
#else
#endif
namespace ab {
	class N_Mapping {
	protected:
		HANDLE handle;
		HANDLE maphandle;
	    int request_mapsize_;
		int current_mapsize_;
		long long filesize_;
		long long offset_;
		void *mapptr;
		unsigned long granul_;
	public:
		N_Mapping(const wchar_t *filename, int mapSize);
		~N_Mapping();
		void* map(long long offset);
		void* map(long long offset, int newMapSize);
		inline long long filesize()
		{
			return filesize_;
		}
		inline int mapsize()
		{
			return current_mapsize_;
		}
		inline int granul()
		{
			return granul_;
		}
	};
}
#endif /*N_Mapping_h*/
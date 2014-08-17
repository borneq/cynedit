#include <N_Mapping.h>

namespace ab {
#ifdef WIN32
N_Mapping::N_Mapping(const wchar_t *filename, int mapSize)
{
	SYSTEM_INFO SystemInfo;
	GetSystemInfo(&SystemInfo);
	granul_ = SystemInfo.dwAllocationGranularity;
	handle = CreateFileW(filename,GENERIC_READ, FILE_SHARE_READ,NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	LARGE_INTEGER size;
	GetFileSizeEx(handle,&size);
	filesize_ = size.QuadPart;
	maphandle = CreateFileMapping(handle, NULL, PAGE_READONLY, 0,0,NULL);
	mapptr = NULL;
	request_mapsize_ = mapSize;
	offset_ = 0;
}

N_Mapping::~N_Mapping()
{
	if (mapptr) UnmapViewOfFile(mapptr);
	CloseHandle(maphandle);
	CloseHandle(handle);
}
void* N_Mapping::map(long long offset)
{
	return map(offset, request_mapsize_);
}
void* N_Mapping::map(long long offset, int newMapSize)
{
	if (newMapSize==0 || newMapSize % granul_ != 0) throw "bad size of mapping";
	if (offset % granul_ != 0) throw "bad offset granulality of mapping";
	if (offset>filesize_) throw "bad offset position of mapping";
	if (offset==offset_ && newMapSize == request_mapsize_ && mapptr!=NULL)
		return mapptr;
	request_mapsize_ = newMapSize;
    if (mapptr) UnmapViewOfFile(mapptr);
	unsigned long dwLow = (unsigned long)offset;
	unsigned long dwHigh = (unsigned long)(offset>>32);
	int msize;
	if (filesize_-offset < request_mapsize_)
	{
		msize = 0;
		current_mapsize_  = (int)(filesize_-offset);
	}
	else
		msize = current_mapsize_ = request_mapsize_;
    mapptr = MapViewOfFile(maphandle, FILE_MAP_READ,dwHigh,dwLow,msize);
	offset_ = offset;
	return mapptr;
}
#else
#endif
}

#include <N_Mapping.h>

namespace ab {
#ifdef WIN32
N_Mapping::N_Mapping(const wchar_t *filename, int mapSize)
{
	SYSTEM_INFO SystemInfo;
	GetSystemInfo(&SystemInfo);
	granul = SystemInfo.dwAllocationGranularity;
	handle = CreateFileW(filename,GENERIC_READ, FILE_SHARE_READ,NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	LARGE_INTEGER size;
	GetFileSizeEx(handle,&size);
	filesize_ = size.QuadPart;
	maphandle = CreateFileMapping(handle, NULL, PAGE_READONLY, 0,0,NULL);
	mapptr = NULL;
	mapsize_ = mapSize;
}
N_Mapping::~N_Mapping()
{
	if (mapptr) UnmapViewOfFile(mapptr);
	CloseHandle(maphandle);
	CloseHandle(handle);
}
void* N_Mapping::map(long long offset)
{
	return map(offset, mapsize_);
}
void* N_Mapping::map(long long offset, int newMapSize)
{
	if (newMapSize==0 || newMapSize % granul != 0) throw "bad size of mapping";
	if (offset % granul != 0) throw "bad offset of mapping";
	mapsize_ = newMapSize;
    if (mapptr) UnmapViewOfFile(mapptr);
	unsigned long dwLow = (unsigned long)offset;
	unsigned long dwHigh = (unsigned long)(offset>>32);	
	int ms;
	if (filesize_-offset < mapsize_)
		ms = 0;
	else 
		ms = mapsize_;
    mapptr = MapViewOfFile(maphandle, FILE_MAP_READ,dwHigh,dwLow,ms);
	return mapptr;
}
#else
#endif
inline long long N_Mapping::size()
{
	return filesize_;
}
}
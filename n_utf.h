#ifndef N_Utf_h
#define N_Utf_h

#include <FL/Fl.H>

namespace ab {
	const int NO_BOM = 0;
	const int BOM_UTF8 = 1;
	const int CODING_LOCALE = 1;
	const int CODING_UTF8 = 1;

	uchar utf8CharLen(uchar *utf8);
	unsigned long utf8to32_one(const char *start, int *len, int maxlen);
	int utf32to8_one(unsigned ucs, char* buf);
	int lenUtf8_one(unsigned ucs);
	unsigned long utf16to32_one(const wchar_t *start, int *len, int maxlen);
	int utf32to16_one(unsigned ucs, wchar_t* buf);
	int lenUtf16_one(unsigned ucs);
	int utf8to32(const char *utf8, int utf8len, int *utf32);
	void utf32to8(const int *utf32, int utf32len, char *utf8);
	int utf16to32(const wchar_t *utf16, int utf16len, int *utf32);
	void utf32to16(const int *utf32, int utf32len, wchar_t *utf16);
	int utf8to16(const char *utf8, int utf8len, wchar_t *utf16);
	int utf16to8(const wchar_t *utf16, int utf16len, char *utf8);
	int lenUtf8to16(const char *utf8, int utf8len);
	wchar_t *allocUtf16for8(const char *utf8);
	bool isUTF8(char *buf, int buflen);
}

#endif /*N_Utf_h*/

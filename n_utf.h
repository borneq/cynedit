namespace ab {
	unsigned long utf8to32_one(char *start, int *len, int maxlen);
	int utf32to8_one(unsigned ucs, char* buf);
	int lenUtf8_one(unsigned ucs);
	unsigned long utf16to32_one(wchar_t *start, int *len, int maxlen);
	int utf32to16_one(unsigned ucs, wchar_t* buf);
	int lenUtf16_one(unsigned ucs);
	int utf8to32(char *utf8, int utf8len, int *utf32);
	void utf32to8(int *utf32, int utf32len, char *utf8);
	int utf16to32(wchar_t *utf16, int utf16len, int *utf32);
	void utf32to16(int *utf32, int utf32len, wchar_t *utf16);
	int utf8to16(char *utf8, int utf8len, wchar_t *utf16);
	int utf16to8(wchar_t *utf16, int utf16len, char *utf8);
}
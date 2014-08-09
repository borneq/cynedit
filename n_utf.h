namespace ab {
unsigned long utf8to32One(char *start, int *len);
int utf32to8One(unsigned ucs, char* buf);
int lenUtf8(unsigned ucs);
unsigned long utf16to32One(wchar_t *start, int *len);
int utf32to16One(unsigned ucs, wchar_t* buf);
int lenUtf16(unsigned ucs);
}
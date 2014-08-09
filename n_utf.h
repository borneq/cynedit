namespace ab {
unsigned long utf8decode(char *start, int *len);
int utf8encode(unsigned ucs, char* buf);
unsigned long utf16decode(wchar_t *start, int *len);
int utf16encode(unsigned ucs, wchar_t* buf);
}
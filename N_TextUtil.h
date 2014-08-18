#ifndef N_TEXTUTIL_H
#define N_TEXTUTIL_H

namespace ab {
const int NO_LINE_END  = 0;
const int LF_UNIX      = 1;
const int CRLF_WINDOWS = 2;
const int CR_MAC       = 3;

bool getNextLineZ(char *text, char*&line, int &pos, int &endType);
bool getNextLine(char *text, int maxLen, char*&line, int &pos, int &endType, int maxLineLen, bool UTF8Align);
void backToBeginLine(char *text, int &pos);
void backToBeginLines(char *text, int &pos, int nLines);
void backToBeginLine(char *text, int &pos, int maxLineLen);
void backToBeginLines(char *text, int &pos, int nLines, int maxLinesLen);
char *mergeLines(char *lineA, char *lineB);
int fixedTabExpand(char *in, char *out, int nVisibleFixedChars, int horizPos, uchar tabWidth, bool tabAlign);
}

#endif /*N_TEXTUTIL_H*/

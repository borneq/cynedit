#ifndef N_TEXTUTIL_H
#define N_TEXTUTIL_H

namespace ab {
const int NO_LINE_END  = 0;
const int LF_UNIX      = 1;
const int CRLF_WINDOWS = 2;
const int CR_MAC       = 3;

bool getNextLine(char *text, char*&line, int &pos, int &endType);
char *mergeLines(char *lineA, char *lineB);
}

#endif /*N_TEXTUTIL_H*/

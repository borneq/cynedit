#include <FL/Fl.H> //uchar
#include <string.h> // memcpy
#include <stdlib.h> //malloc
#include <stdio.h> //NULL
#include <N_Utf.h>
#include <N_TextUtil.h>

namespace ab {

//not so obvious case when line break at end or not
//empty text has one empty line, nil (for example no open stream) - no lines
//if Pos points to ending #0 but string ends by space is not end yet
//stop if 0
bool getNextLineZ(char *text, char*&line, int &pos, int &endType)
{
char *head,*tail;
  endType = NO_LINE_END;
  line = NULL;
  if (text==NULL) return false;
  head = text + pos;
  tail = head;
  if (head>text && head[-1]==NULL) return false;

  while (*tail != 10 && *tail != 13 && *tail != 0) tail++;
  line = (char*)malloc(tail-head+1);
  memcpy ( line, head, tail-head );
  line[tail-head] = 0;

  if (*tail==0)endType = NO_LINE_END;
  else
  {
	  if (*tail==10) endType = LF_UNIX;
	  else if (*tail==13) endType = CR_MAC;
	  tail++; //for Unix line ending
	  if (*tail==10 && tail[-1]==13)
	  {
		  endType = CRLF_WINDOWS;
		  tail++; //for Windows line endings
	  }
  }
  pos = tail-text;
  return true;
}

//stop if tail=maxlen
bool getNextLine(char *text, int maxLen, char*&line, int &pos, int &endType, int maxLineLen, bool UTF8Align)
{
char *head,*tail;
  endType = NO_LINE_END;
  line = NULL;
  if (text==NULL) return false;
  head = text + pos;
  tail = head;
  if (head>text && head[-1]==NULL) return false;

  while (tail-head<maxLineLen && tail-text<maxLen && *tail != 10 && *tail != 13) tail++;
  if (UTF8Align)
  { //go back but no more than to head+1
	  unsigned char *p = (unsigned char *)(tail-1);
	  int d = 0;
	  while (*p >=0x80 && *p <= 0xBF)
	  {
		  p--;
		  d++;
		  if (d==5) break;
	  }
	  int need_d;
	  if (*p>=0xC0 && *p<=0xDF) need_d=1; //110xxxxx
	  else if (*p>=0xE0 && *p<=0xEF) need_d=2; //1110xxxx
	  else if (*p>=0xF0 && *p<=0xF7) need_d=3; //11110xxx
	  else if (*p>=0xF8 && *p<=0xFB) need_d=4; //111110xx
	  else if (*p>=0xFC) need_d=5; //1111110x
	  else need_d=0;
	  if (d<need_d && (char*)p>head) tail = (char*)p;
    /*
		0xxxxxxx
		110xxxxx	10xxxxxx
		1110xxxx	10xxxxxx	10xxxxxx
		11110xxx	10xxxxxx	10xxxxxx	10xxxxxx
		111110xx	10xxxxxx	10xxxxxx	10xxxxxx	10xxxxxx
		1111110x	10xxxxxx	10xxxxxx	10xxxxxx	10xxxxxx	10xxxxxx
		*/
  }

  line = (char*)malloc(tail-head+1);
  memcpy ( line, head, tail-head );
  line[tail-head] = 0;

  if (tail-text>=maxLen) endType = NO_LINE_END;
  else
  {
	if (*tail==10) endType = LF_UNIX;
	else if (*tail==13) endType = CR_MAC;
	else endType = NO_LINE_END;
	if (endType!=NO_LINE_END)
	{
		tail++; //for Unix line ending
		if (tail-text<maxLen)
		if (*tail==10 && tail[-1]==13)
		{
			endType = CRLF_WINDOWS;
			tail++; //for Windows line endings
		}
	}
  }
  pos = tail-text;
  return true;
}

void backToBeginLine(char *text, int &pos)
{
	if (text[pos]==10 && pos-1>=0 && text[pos-1]==13) pos--;
	if (pos>0 &&(text[pos]==13 || text[pos]==10)) pos--;
	while (pos >= 0)
	{
		if (text[pos]==13 || text[pos]==10)
		{
			break;
		}
		pos--;
	}
	pos++;
}


void backToBeginLines(char *text, int &pos, int nLines)
{
	for (int i=0; i<nLines; i++)
	{
		if (text[pos]==10 && pos-1>=0 && text[pos-1]==13) pos--;
		if (pos>0 &&(text[pos]==13 || text[pos]==10)) pos--;
		while (pos >= 0)
		{
			if (text[pos]==13 || text[pos]==10)
			{
				break;
			}
			pos--;
		}
		if (pos<0) break;
	}
	pos++;
}

void backToBeginLine(char *text, int &pos, int maxLineLen)
{
	int savpos = pos;
	if (text[pos]==10 && pos-1>=0 && text[pos-1]==13) pos--;
	if (pos>0 &&(text[pos]==13 || text[pos]==10)) pos--;
	while (pos >= 0 && savpos-pos<maxLineLen)
	{
		if (text[pos]==13 || text[pos]==10)
		{
			break;
		}
		pos--;
	}
	pos++;
}

void backToBeginLines(char *text, int &pos, int nLines, int maxLinesLen)
{
	int savpos = pos;
	int lastLineBegin=0;
	for (int i=0; i<nLines; i++)
	{
		if (text[pos]==10 && pos-1>=0 && text[pos-1]==13) pos--;
		if (pos>0 &&(text[pos]==13 || text[pos]==10)) pos--;
		while (pos >= 0 && savpos-pos<maxLinesLen)
		{
			if (text[pos]==13 || text[pos]==10)
			{
				lastLineBegin = pos+1;
				break;
			}
			pos--;
		}
		if (pos<0) break;
		if (savpos-pos>=maxLinesLen) break;
	}
	pos++;
	if (lastLineBegin!=0 && pos!=0) pos=lastLineBegin;
}

//alloc space to lineA+lineB ana dfree lineA,lineB
char *mergeLines(char *lineA, char *lineB)
{
	int lenA = strlen(lineA);
	int lenB = strlen(lineB);
	char *res = (char*)malloc(lenA+lenB+1);
	memcpy(res, lineA, lenA);
	memcpy(res+lenA, lineB, lenB);
	res[lenA+lenB] = 0;
	free(lineA);
	free(lineB);
	return res;
}

//tab expanding for fixed font
//copy maximal nVisibleFixedChars utf8 chars from in to out, one utf8 char can contains one or more chars
int fixedTabExpandUtf8(char *in, char *out, int nVisibleFixedChars, int horizPos, uchar tabWidth, bool tabAlign)
{
	int nChar=0;
	int result=0;
	while (nChar<horizPos && *in!=0)
	{
		if (*in==9)
		{
			if (tabAlign)
			{
				do{
					nChar++;
				} while(nChar % tabWidth != 0 && nChar<horizPos);
				while(nChar % tabWidth != 0 && nChar<nVisibleFixedChars+horizPos)
				{
					*out = ' ';
				    out++;
					nChar++;
				}
			}
			else
			{
				int tw = tabWidth;
				for (int i=0;i<tabWidth && nChar<horizPos;i++)
				{
					nChar++;
					tw--;
				}
				for (int i=0;i<tw && nChar<nVisibleFixedChars+horizPos;i++)
				{
					*out = ' ';
				    out++;
					nChar++;
				}
			}
			in++;
		}
		else
		{
			uchar len=utf8CharLen((uchar)*in);
			for (int i=0;i<len;i++)
			{
				if (*in==0) break;
				in++;
			}
			nChar++;
		}
	}
	nChar -= horizPos;

	while (nChar<nVisibleFixedChars && *in!=0)
	{
		if (*in==9)
		{
			if (tabAlign)
			do{
				*out = ' ';
				out++;
				result++;
				nChar++;
			} while((nChar+horizPos) % tabWidth != 0 && nChar<nVisibleFixedChars);
			else
			for (int i=0;i<tabWidth && nChar<nVisibleFixedChars;i++)
			{
				*out = ' ';
				out++;
				result++;
				nChar++;
			}
			in++;
		}
		else
		{
			uchar len=utf8CharLen((uchar)*in);
			for (int i=0;i<len;i++)
			{
				if (*in==0) break;
				*out = *in;
				in++;
				out++;
			}
			result +=len;
			nChar++;
		}
	}
	return result;
}

//version UCS4
int fixedTabExpand(int *in, int inLen, int *out, int nVisibleFixedChars, int horizPos, uchar tabWidth, bool tabAlign)
{
	int nChar=0;
	int result=0;
	int inPos = 0;
	while (nChar<horizPos && inPos<inLen)
	{
		if (*in==9)
		{
			if (tabAlign)
			{
				do{
					nChar++;
				} while(nChar % tabWidth != 0 && nChar<horizPos);
				while(nChar % tabWidth != 0 && nChar<nVisibleFixedChars+horizPos)
				{
					*out = 32;
				    out++;
					nChar++;
				}
			}
			else
			{
				int tw = tabWidth;
				for (int i=0;i<tabWidth && nChar<horizPos;i++)
				{
					nChar++;
					tw--;
				}
				for (int i=0;i<tw && nChar<nVisibleFixedChars+horizPos;i++)
				{
					*out = 32;
				    out++;
					nChar++;
				}
			}
		}
		else
		{
			nChar++;
		}
		in++;
		inPos++;
	}
	nChar -= horizPos;

	while (nChar<nVisibleFixedChars && inPos<inLen)
	{
		if (*in==9)
		{
			if (tabAlign)
			do{
				*out = 32;
				out++;
				result++;
				nChar++;
			} while((nChar+horizPos) % tabWidth != 0 && nChar<nVisibleFixedChars);
			else
			for (int i=0;i<tabWidth && nChar<nVisibleFixedChars;i++)
			{
				*out = 32;
				out++;
				result++;
				nChar++;
			}
		}
		else
		{
			*out = *in;
			out++;
			result++;
			nChar++;
		}
		in++;
		inPos++;
	}
	return result;
}
}

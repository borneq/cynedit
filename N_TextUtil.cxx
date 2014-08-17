#include <string.h> // memcpy
#include <stdlib.h> //malloc
#include <stdio.h> //NULL
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
	if (lastLineBegin!=0) pos=lastLineBegin;
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
}

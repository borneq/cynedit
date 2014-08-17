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

  if (*tail==10) endType = LF_UNIX;
  else if (*tail==13) endType = CR_MAC;
  else endType = NO_LINE_END;
  tail++; //for Unix line ending
  if (*tail==10 && tail[-1]==13)
  {
	  endType = CRLF_WINDOWS;
	  tail++; //for Windows line endings
  }
  pos = tail-text;
  return true;
}

//stop if tail=maxlen
bool getNextLine(char *text, int maxlen, char*&line, int &pos, int &endType)
{
char *head,*tail;
  endType = NO_LINE_END;
  line = NULL;
  if (text==NULL) return false;
  head = text + pos;
  tail = head;
  if (head>text && head[-1]==NULL) return false;

  while (tail-text<maxlen && *tail != 10 && *tail != 13) tail++;
  line = (char*)malloc(tail-head+1);
  memcpy ( line, head, tail-head );
  line[tail-head] = 0;

  if (*tail==10) endType = LF_UNIX;
  else if (*tail==13) endType = CR_MAC;
  else endType = NO_LINE_END;
  tail++; //for Unix line ending
  if (*tail==10 && tail[-1]==13)
  {
	  endType = CRLF_WINDOWS;
	  tail++; //for Windows line endings
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

#include <string.h> // memcp
#include <stdlib.h> //malloc
#include <stdio.h> //NULL

//not so obvious case when line break at end or not
//empty text has one empty line, nil (for example no open stream) - no lines
//if Pos points to ending #0 but string ends by space is not end yet
bool getNextLine(char *text, char*&line, int &pos)
{
char *head,*tail;  
  line = NULL;
  if (text==NULL) return false;
  head = text + pos;
  tail = head;
  if (head>text && head[-1]==NULL) return false;
    
  while (*tail != 10 && *tail != 13 && *tail != NULL) tail++;
  line = (char*)malloc(tail-head+1);
  memcpy ( line, head, tail-head );
  line[tail-head] = 0;
  
  tail++; //for Unix line ending
  if (*tail==10 && tail[-1]==13) tail++; //for Windows line endings
  pos = tail-text;
  return true;
}

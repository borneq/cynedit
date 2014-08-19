#ifndef N_UTF_H
#define N_UTF_H

#include <assert.h>
#include <n_utf.h>
#include <cstring>
#include <stdlib.h>
#ifdef WIN32
#include <Windows.h>
#else
#endif

namespace ab {
/*! Decode a single UTF-8 encoded character starting at \e start
	after decoding set \e len to length this character
	maxlen can be \e 1 whereas len should be greater
	in this case function returns zero. This occurs when UTF8 text
	is divided to blocks and block division not fit to uutf8 division
*/

uchar utf8CharLen(uchar firstByte)
{
	if ((firstByte & 0x80) == 0) return 1;
	if ((firstByte & 0x20) == 0) return 2;
	if ((firstByte & 0x10) == 0) return 3;
	return 4;
}

unsigned long utf8to32_one(const char *start, int *len, int maxlen)
{
uchar b0,b1,b2,b3;
	*len = 1;
	b0 = start[0];
	if ((b0 & 0x80) == 0) return b0;
	if ((b0 & 0x20) == 0)
	{
		*len = 2;
		if (maxlen<*len) return 0;
		b1 = start[1];
		return (b1 & 63) | ((b0 & 31)<< 6);
	}
	if ((b0 & 0x10) == 0)
	{
		*len = 3;
		if (maxlen<*len) return 0;
		b1 = start[1];
		b2 = start[2];
		return (b2 & 63) | ((b1 & 63)<< 6)| ((b0 & 15) << 12);
	}
	*len = 4;
	if (maxlen<*len) return 0;
	b1 = start[1];
	b2 = start[2];
	b3 = start[3];
	return (b3 & 63)| ((b2 & 63)<< 6) | ((b1 & 63) << 12) | ((b0 & 7) << 18);
}

//function from FLTK package
/*! Write the UTF-8 encoding of \e ucs into \e buf and return the
    number of bytes written. Up to 4 bytes may be written. If you know
    that \p ucs is less than 0x10000 then at most 3 bytes will be written.
    If you wish to speed this up, remember that anything less than 0x80
    is written as a single byte.

    If ucs is greater than 0x10ffff this is an illegal character
    according to RFC 3629. These are converted as though they are
    0xFFFD (REPLACEMENT CHARACTER).

    RFC 3629 also says many other values for \p ucs are illegal (in
    the range 0xd800 to 0xdfff, or ending with 0xfffe or
    0xffff). However I encode these as though they are legal, so that
    utf8encode/fl_utf8decode will be the identity for all codes between 0
    and 0x10ffff.
*/
int utf32to8_one(unsigned ucs, char* buf)
{
  if (ucs < 0x000080U) {
    buf[0] = ucs;
    return 1;
  } else if (ucs < 0x000800U) {
    buf[0] = 0xc0 | (ucs >> 6);
    buf[1] = 0x80 | (ucs & 0x3F);
    return 2;
  } else if (ucs < 0x010000U) {
    buf[0] = 0xe0 | (ucs >> 12);
    buf[1] = 0x80 | ((ucs >> 6) & 0x3F);
    buf[2] = 0x80 | (ucs & 0x3F);
    return 3;
  } else if (ucs <= 0x0010ffffU) {
    buf[0] = 0xf0 | (ucs >> 18);
    buf[1] = 0x80 | ((ucs >> 12) & 0x3F);
    buf[2] = 0x80 | ((ucs >> 6) & 0x3F);
    buf[3] = 0x80 | (ucs & 0x3F);
    return 4;
  } else {
    /* encode 0xfffd: */
    buf[0] = 0xefU;
    buf[1] = 0xbfU;
    buf[2] = 0xbdU;
    return 3;
  }
}

/*! get length utf8 string of one UCS character*/
int lenUtf8_one(unsigned ucs)
{
  if (ucs < 0x000080U)
    return 1;
  else if (ucs < 0x000800U)
    return 2;
  else if (ucs < 0x010000U)
    return 3;
  else if (ucs <= 0x0010ffffU)
    return 4;
  else
    /* encode 0xfffd: */
    return 3;
}

/*!
	Decode pair surrogates to one UCS
*/
unsigned long utf16to32_one(const wchar_t *start, int *len, int maxlen)
{
	assert(maxlen>=1 && maxlen<=2);
	if (start[0]>=0xD800 && start[0]<=0xDFFF)
	{
		*len = 2;
		if (maxlen<*len) return 0;
		return 0x400*(start[0]-0xD800)+(start[1]-0xDC00)+0x10000;
	}
	else
	{
		*len = 1;
		return start[0];
	}
}

///returns length = 1 or 2
int utf32to16_one(unsigned ucs, wchar_t* buf)
{
	if (ucs<0x10000)
    {
		if (ucs>=0xD800 && ucs<=0xDFFF)
			buf[0] = 0xfffd;
		else
			buf[0] = (wchar_t) ucs;
		return 1;
	}
	else if(ucs <= 0x0010ffffU)
	{
      buf[0] = ((ucs-0x10000) >>10) + 0xD800;
      buf[1] = ((ucs-0x10000) & 1023) + 0xDC00;
	  return 2;
	}
	else
	{
		buf[0] = 0xfffd;
		return 1;
	}
}

/*! get length utf16 string of one UCS character*/
int lenUtf16_one(unsigned ucs)
{
	if (ucs<0x10000)
    	return 1;
	else if(ucs <= 0x0010ffffU)
	  return 2;
	else
      return 1;
}

///returns length of UCS4 string
int utf8to32(const char *utf8, int utf8len, int *utf32)
{
	int len=0;
	while (utf8len>0)
	{
		int charlen;
		*utf32 = utf8to32_one(utf8, &charlen, utf8len);
		if (charlen==0) return len+1;
		utf8 += charlen;
		utf32++;
		len++;
		utf8len -= charlen;
	}
	return len;
}

///returns length of UTF8 string
int utf32to8(const int *utf32, int utf32len, char *utf8)
{
	int len=0;
	for (int i=0; i<utf32len; i++)
	{
		int charlen = utf32to8_one(utf32[i], utf8);
		utf8 += charlen;
		len += charlen;
	}
	return len;
}

///returns length of UCS4 string
int utf16to32(const wchar_t *utf16, int utf16len, int *utf32)
{
	int len=0;
	while (utf16len>0)
	{
		int charlen;
		*utf32 = utf16to32_one(utf16, &charlen, utf16len);
		if (charlen==0) return len+1;
		utf16 += charlen;
		utf32++;
		len++;
		utf16len -= charlen;
	}
	return len;
}

///returns length of UTF16 string
int utf32to16(const int *utf32, int utf32len, wchar_t *utf16)
{
	int len=0;
	for (int i=0; i<utf32len; i++)
	{
		int charlen = utf32to16_one(utf32[i], utf16);
		utf16 += charlen;
		len += charlen;
	}
	return len;
}

///returns length of UTF16 string
int utf8to16(const char *utf8, int utf8len, wchar_t *utf16)
{
	int len=0;
	while (utf8len>0)
	{
		int len8;
		unsigned long ucs = utf8to32_one(utf8, &len8, utf8len);
		if (len8==0) return len+1;
		utf8 += len8;
		utf8len -= len8;
		int len16 = utf32to16_one(ucs, utf16);
		utf16 += len16;
		len += len16;
	}
	return len;
}

int lenUtf8to16(const char *utf8, int utf8len)
{
	int sumLen16 = 0;
	int len8;
	while (utf8len>0)
	{
		unsigned long ucs = utf8to32_one(utf8, &len8, utf8len);
		if (len8==0) return sumLen16+1;
		utf8 += len8;
		utf8len -= len8;
		int len16 = lenUtf16_one(ucs);
		sumLen16 += len16;
	}
	return sumLen16;
}


int utf16to8(const wchar_t *utf16, int utf16len, char *utf8)
{
	int len=0;
	while (utf16len>0)
	{
		int len16;
		unsigned long ucs = utf16to32_one(utf16, &len16, utf16len);
		if (len16==0) return len+1;
		utf16 += len16;
		utf16len -= len16;
		int len8 = utf32to8_one(ucs, utf8);
		utf8 += len8;
	}
	return len;
}


wchar_t *allocUtf16for8(const char *utf8)
{
	int len = strlen(utf8);
	int lenW = lenUtf8to16(utf8, len);
	wchar_t *wbuf = (wchar_t *) malloc(sizeof(wchar_t)*(lenW+1));
	utf8to16(utf8, len, wbuf);
	return wbuf;
}

//auxiliary function to isUTF8
void nextByteUTF8(char *buf, int &pos, int d, int &UTFcnt, int &nonUTFcnt)
{
	pos++;
	unsigned char c = buf[pos];
	if (c >= 0x80 && c <= 0xBF)
	{
		if (d == 0) UTFcnt++;
	}
	else nonUTFcnt++;
}

bool isUTF8(char *buf, int buflen)
{
	int UTFcnt = 0, nonUTFcnt = 0;
	int i=0;
	int d;
	while (i < buflen)
	{
		unsigned char c = buf[i];
		if (c < 0x7F); //Latin: nothing do
		else if (c >= 0xC0 && c <= 0xDF)// 110xxxxx 10xxxxxx
		{
			if (i+1 < buflen) nextByteUTF8(buf, i, 0, UTFcnt, nonUTFcnt);
		}
		else if (c >= 0xE0 && c <= 0xEF) // 1110xxxx 10xxxxxx 10xxxxxx
		{
		    d = 1;
			while (i + 1 < buflen && d >= 0)
			{
				nextByteUTF8(buf, i, d, UTFcnt, nonUTFcnt);
				d--;
			}
		}
		else if (c >= 0xF0 && c <= 0xF7)  // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
		{
			d = 1;
			while (i + 1 < buflen && d >= 0)
			{
				nextByteUTF8(buf, i, d, UTFcnt, nonUTFcnt);
				d--;
			}
		}
		else
			nonUTFcnt++;
		i++;
	}
	return UTFcnt > nonUTFcnt;
}

#ifdef WIN32
int len_locale_to_utf16(char *utf8, int utf8len)
{
	return MultiByteToWideChar(CP_ACP, 0, utf8, utf8len, NULL, 0);
}

int locale_to_utf16(char *utf8, int utf8len, wchar_t *utf16, int utf16len)
{
	return MultiByteToWideChar(CP_ACP, 0, utf8, utf8len, utf16, utf16len);
}

int len_utf16_to_locale(wchar_t *utf16, int utf16len)
{
	return WideCharToMultiByte(CP_ACP, 0, utf16, utf16len, NULL, 0, NULL, NULL);
}

int utf16_to_locale(wchar_t *utf16, int utf16len, char *utf8, int utf8len)
{
	return WideCharToMultiByte(CP_ACP, 0, utf16, utf16len, utf8, utf8len, NULL, NULL);
}

#else
iconv_open , nl_langinfo
http ://www.gnu.org/software/libc/manual/html_node/Generic-Conversion-Interface.html
http ://www.gnu.org/software/libc/manual/html_node/iconv-Examples.html
#endif


}
#endif /*N_UTF_H*/

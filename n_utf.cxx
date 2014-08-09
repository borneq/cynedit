namespace ab {
/*! Decode a single UTF-8 encoded character starting at \e start
	after decoding set \e len to length this character
*/
unsigned long utf8decode(char *start, int *len)
{
unsigned char b0,b1,b2,b3;
	*len = 1;
	b0 = start[0];
	if ((b0 & 0x80) == 0) return b0;
	if ((b0 & 0x20) == 0)
	{
		*len = 2;
		b1 = start[1];
		return (b1 & 63) | ((b0 & 31)<< 6);
	}
	if ((b0 & 0x10) == 0)
	{
		*len = 3;
		b1 = start[1];
		b2 = start[2];
		return (b2 & 63) | ((b1 & 63)<< 6)| ((b0 & 15) << 12);
	}
	*len = 4;
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
int utf8encode(unsigned ucs, char* buf) 
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

/*! 
	Decode pair surrogates to one UCS
*/
unsigned long utf16decode(wchar_t *start, int *len)
{
	if (start[0]>=0xD800 && start[0]<=0xDFFF)
	{
		*len = 1;
		return 0x400*(start[0]-0xD800)+(start[1]-0xDC00)+0x10000;
	}
	else
	{
		*len = 1;
		return start[0];
	}
}

///returns length = 1 or 2
int utf16encode(unsigned ucs, wchar_t* buf) 
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
}
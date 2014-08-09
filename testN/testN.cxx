#include <memory.h>
#include <stdio.h>
#include <T_List.hpp>
#include <T_Stack.hpp>
#include <T_Queue.hpp>
#include <N_File_Stream.h>
#include <n_utf.h>
#include <N_StringList.h>
#include <N_TextUtil.h>

using namespace ab;

void test_list()
{
	T_List<char> list(true);
	list.add(0);
	list.add(1);
	list.add(2);
	list.add(3);
	list.del(1);
	int old_capacity = -1;
	for (int i=0;i<10000; i++)
	{
		if (list.capacity()!=old_capacity)
			printf("size=%d, capacity=%d\n",list.size(),list.capacity());
		old_capacity = list.capacity();
		list.add(0);
	}

	printf("capacity=%d, now delete\n",list.capacity());
	for (int i=0;i<10000; i++)
	{
		if (list.capacity()!=old_capacity)
			printf("size=%d, capacity=%d\n",list.size(),list.capacity());
		old_capacity = list.capacity();
		list.del(rand() % list.size());
	}
}

void test_stack()
{
	T_Stack<char> stack(true);
	stack.push(0);
	stack.push(1);
	stack.push(2);
	printf("pop=%d\n",stack.pop());
	printf("pop=%d\n",stack.pop());
	printf("pop=%d\n",stack.pop());
}

void test_queue()
{
	T_Queue<int> queue(true);
	for (int i=0; i<100; i++) 
		queue.push(i);
	for (int i=0; i<100; i++) 
	    printf("pop=%d\n",queue.pop());	
	for (int i=0; i<25; i++) 
		queue.push(i);
	for (int i=0; i<10; i++) 
        printf("pop=%d\n",queue.pop());	
}

void test_stream()
{
	N_File_Stream outStream("abc.000","w");
}


void test_ucs()
{
	char buf[4];
	wchar_t wbuf[2];

	for (int i=0; i<=0x10ffff; i++)
	{
	  utf32to8_one(i, buf);
	  int len;
	  int n=utf8to32_one(buf,&len,4);
	  if (n!=i) throw "bad utf8 conversion";
	  if (i<0xD800 || i>0xDFFF)
	  {
		  utf32to16_one(i, wbuf);
		  n=utf16to32_one(wbuf,&len,2);
		  if (n!=i) 
			  throw "bad utf8 conversion";
	  }
	}
}

void show(char *strA)
{
	int pos = 0;
	char *line;
    while (getNextLine(strA,line,pos)) printf("%d : \"%s\" ",pos,line);
	printf("\n");
	free(line);
}

void test_getNextLine()
{
	char *strA;
	strA = "";				// ""
	show(strA);
	strA = "\r";			// "",""
	show(strA);
	strA = "abc";			// "abc"
	show(strA);
	strA = "\rabc";			// "","abc"
	show(strA);
	strA = "abc\rdef";		// "abc","def"
	show(strA);
	strA = "abc\rdef\r";	// "abc","def",""
	show(strA);
	strA = "abc\r\rdef";	// "abc","","def"
	show(strA);
	strA = "abc\r\r";		// "abc","",""
	show(strA);
	strA = "abc\rdef\r\n123\n";		// "abc","def","123",""
	show(strA);
	strA = "abc\rdef\r\n\r123\n";	// "abc","def","","123",""
	show(strA);
	strA = "abc\rdef\r\r123";		// "abc","def","","123"	
	show(strA);
}
 
void test_stringlist()
{
	N_StringList list;
	list.loadFromFile(L"c:\\temp\\test.txt");
	for (int i=0; i<list.size(); i++)
		printf("%s\n",list[i]);
}

int main(int argc, char** argv) {	
	//test_list();
	//test_stack();
	//test_queue();
	//test_ucs();
	//test_getNextLine();
	test_stringlist();
	getchar();
}
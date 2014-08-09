#include <memory.h>
#include <stdio.h>
#include <T_List.hpp>
#include <T_Stack.hpp>

using namespace afltk;

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
int main(int argc, char** argv) {	
	//test_list();
	test_stack();
	getchar();
}
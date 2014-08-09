#include <memory.h>
#include <stdio.h>
#include <T_List.hpp>
#include <T_Stack.hpp>
#include <T_Queue.hpp>

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

int main(int argc, char** argv) {	
	//test_list();
	//test_stack();
	test_queue();
	getchar();
}
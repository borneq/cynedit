///functionality of stack is subset of list functionality
#ifndef T_STACK_H
#define T_STACK_H
#include <T_List.hpp>

namespace afltk {
template<typename T>
class T_Stack : protected T_List<T> {
public:
	T_Stack(bool auto_shrink=false): T_List(auto_shrink){};

	inline void push(T item)
	{
		add(item);
	}
	inline T pop()
	{
		if (_size==0) throw "T_Stack.pop exception - empty stack";
		T ret  = _list[_size-1];
		del(_size-1);
		return ret;
	}
};
}
#endif /*T_STACK_H*/

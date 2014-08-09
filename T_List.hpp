#ifndef T_LIST_H
#define T_LIST_H

#include <stdlib.h>
#include "N_Base_List.H"

namespace afltk {
/// B_List is template class of list
template<typename T>
class T_List {
protected:
	T	*_list;
	int _capacity;
	int _size; ///
	virtual void grow()
	{
	   int oldCapacity = _capacity;
	   void **oldList = _list;
	   _capacity = grow_capacity(_capacity);
	   _list = (void **)malloc(_capacity * sizeof(void*));
	   assert(_capacity>oldCapacity);
	   if (oldList!=NULL)
		 memcpy(_list, oldList, oldCapacity*sizeof(void*));
	   free(oldList);
	}

public:
	void add(T item)
	{
		if (_size >= _capacity)
		  grow();
		_list[_size] = item;
		_size++;
	}

	inline bool is_empty(){return _size == 0;}
	virtual void clear()
	{
		free(_list);
		_list = NULL;
		_capacity = 0;
		_size = 0;
	}

	inline int size(){return _size;}
	inline T operator[](int n) {return _list[n];}
	T_List() { _list=NULL; _capacity=0; _size=0;}
    ~T_List(){clear();}
};
}

#endif /*T_LIST_H*/

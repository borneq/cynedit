#ifndef T_LIST_H
#define T_LIST_H

#include <string.h> // memcpy
#include <stdlib.h> //malloc
#include <assert.h>
#include <N_Base_List.H>

namespace afltk {
/// B_List is template class of list
template<typename T>
class T_List {
protected:
	T	*_list;
	int _capacity;
	int _size; ///
	bool _auto_shrink; /// if set  - automatically shrink capacity when size shrink
	int _shrink_trigger;
	virtual void grow()
	{
	   int oldCapacity = _capacity;
	   T *oldList = _list;
	   _capacity = grow_capacity(_capacity);
	   _list = (T*)malloc(_capacity * sizeof(T));
	   assert(_capacity>oldCapacity);
	   if (oldList!=NULL)
		 memcpy(_list, oldList, oldCapacity*sizeof(T));
	   free(oldList);
	   if(_auto_shrink) _shrink_trigger = compute_shrink_trigger(_capacity);
	}


	virtual void shrink()
	{
	#ifdef _DEBUG
		int oldCapacity = _capacity;
	#endif
		T *oldList = _list;
		_capacity = grow_capacity(_size - 1);
		_list = (T*)malloc(_capacity * sizeof(T));
	#ifdef _DEBUG
		assert(_capacity<oldCapacity);
	#endif
		assert(oldList!=NULL);
		memcpy(_list, oldList, _capacity*sizeof(T));
		free(oldList);
		if(_auto_shrink) _shrink_trigger = compute_shrink_trigger(_capacity);
	}


public:
	void add(T item)
	{
		if (_size >= _capacity)
		  grow();
		_list[_size] = item;
		_size++;
	}

	void del(int index)
	{
		if (index<0 || index>=_size) return;
		memmove(&_list[index], &_list[index+1], (_size-index-1)*sizeof(T));
		_size--;
		if (_auto_shrink &&_size>0 && _size <= _shrink_trigger)
			shrink();
	}

	inline bool is_empty(){return _size == 0;}
	virtual void clear()
	{
		free(_list);
		_list = NULL;
		_capacity = 0;
		_size = 0;
		_shrink_trigger = 0;
	}

	inline int size(){return _size;}
	inline int capacity(){return _capacity;}
	inline T operator[](int n) {return _list[n];}
	T_List(bool auto_shrink=false)
	{
		_list=NULL;
		_capacity=0;
		_size=0;
		_auto_shrink = auto_shrink;
		_shrink_trigger = 0;
	}
    ~T_List(){clear();}
};
}

#endif /*T_LIST_H*/

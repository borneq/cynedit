#ifndef T_LIST_H
#define T_LIST_H

#include <string.h> // memcpy
#include <stdlib.h> //malloc
#include <assert.h>
#include <N_Base_List.H>

namespace ab {
/// B_List is template class of list
template<typename T>
class T_List {
protected:
	T	*_list;
	int _capacity;
	int _size; ///
	bool _auto_shrink; /// if set  - automatically shrink capacity when size shrink
	int _shrink_trigger;
	virtual void grow(int gapPos, int gapLen)
	{
	   int oldCapacity = _capacity;
	   T *oldList = _list;
	   _capacity = grow_capacity(_capacity);
	   _list = (T*)malloc(_capacity * sizeof(T));
	   assert(_capacity>oldCapacity);
	   if (_list==NULL) throw "can't malloc";
	   if (oldList!=NULL)
	   {
			memcpy(_list, oldList, gapPos*sizeof(T));
			memcpy(_list+gapPos+gapLen, oldList+gapPos, (oldCapacity-gapPos)*sizeof(T));
			free(oldList);
	   }
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
	///if first>second return 1 else if first<second return -1 else if equal return zero
	int (*compareP)(T&,T&);

	void add(T item)
	{
		assert(_size<=_capacity);
		if (_size == _capacity)
			grow(_size, 1);
		_list[_size] = item;
		_size++;
	}

	void insert(int index, T item)
	{
		assert(_size<=_capacity);
		if (_size == _capacity)
			grow(index, 1);
		_list[index] = item;
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

	struct StackItem
	{
		long first;
		long last;
	};

	/* Based on a non-recursive QuickSort from the SWAG-Archive.
	 TV Sorting Unit by Brad Williams
	 TLongIntList.Sort from Synedita
	 the same mwLongIntList from GExperts */
	void sort()
	{
		sort(0, _size-1);
	}

	void sort(int first, int last)
	{
	  long Left, Right, SubArray, SubLeft, SubRight;
	  T Temp, Pivot;
	  StackItem Stack[32];

	  if (compareP==NULL) throw "must be compareP function pointer";
	  if (_size>1)
	  {
		SubArray = 0;
		Stack[SubArray].first = first;
		Stack[SubArray].last = last;
		do
		{
		  Left = Stack[SubArray].first;
		  Right = Stack[SubArray].last;
		  SubArray--;
		  do
		  {
			SubLeft = Left;
			SubRight = Right;
			Pivot = _list[(Left + Right) >> 1];
			do
			{
			  while (compareP(_list[SubLeft], Pivot)<0) SubLeft++;
			  while (compareP(Pivot, _list[SubRight])<0) SubRight--;
			  if (SubLeft <= SubRight)
			  {
				Temp = _list[SubLeft];
				_list[SubLeft] = _list[SubRight];
				_list[SubRight] = Temp;
				SubLeft++;
				SubRight--;
			  }
			}while  (SubLeft <= SubRight);
			if (SubLeft < Right)
			{
			  SubArray++;
			  Stack[SubArray].first = SubLeft;
			  Stack[SubArray].last = Right;
			}
			Right = SubRight;
		  }while( Left < Right);
		}
		while (SubArray >= 0);
	  }
	}

	//if not found - return index next element
	bool binaryFind(T item, int &index)
	{
	 int L,R;
	 int res;
		index = 0;
		if (_size<1) return false;
		L = 0;
		R = _size-1;
		while (L <= R)
		{
			index = (L + R) >> 1;
			res = compareP(_list[index], item);
			if (res<0) L = index + 1;
			else if (res>0) R = index - 1;
			else return true;
		}
		index = L;
		return false;
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

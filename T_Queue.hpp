#pragma once

#include <exception>
#include <assert.h>
#include <N_Base_List.h>

namespace ab {
template<typename T>
/// Queue: items are added to tail and are removed from the Head.
/// if _head=_tail -> Queue empty
/// if _head<_tail -> normal situation, Count=_tail-_head, data: _head.._tail-1
/// if _head>_tail -> wrapping on cyclic buffer, Count=_capacity-(_head-_tail), data: _head..Capacity-1 + 0.._tail-1
/// if we Push and will _head=_tail, it must be Grow
/// if _head<=_tail we Grow and Shrink normally
/// if _head>_tail and Grow - copy first buffer position to end, dec _tail or wrap it to end
/// if _head>_tail and Shrink - dec Head, move itema after head, decreease gap
class T_Queue {
protected:
	T	*_list;
	int _capacity;
	bool _auto_shrink; /// if set  - automatically shrink capacity when size shrink
	int _shrink_trigger;
	int _head;
	int _tail;

	void growshrink(bool bShrink)
	{
		assert(_tail!=_head || _head==0 && _tail==0);
		int newCapacity;
		if (bShrink)
			newCapacity = grow_capacity(size()- 1);
		else
		    newCapacity = grow_capacity(_capacity);
		T *newList = (T*)malloc(newCapacity * sizeof(T));
		if (newList==NULL) throw("can't malloc");
		//growshrink() uses the situation to normalize: _head=0, _tail>_head
		//which significantly simplifies growshrink() issue
		if (_head<=_tail)
		{
			memcpy(newList, &_list[_head], (_tail-_head)*sizeof(T));
		}
		else
		{
			memcpy(newList, &_list[_head], (_capacity-_head)*sizeof(T));
			memcpy(&newList[_capacity-_head], _list, _tail*sizeof(T));
		}
		free(_list);
		_tail = size();
		_head = 0;
		_capacity = newCapacity;
		_list = newList;
		_shrink_trigger = compute_shrink_trigger(_capacity);
	}

 public:
	T_Queue(bool auto_shrink=false)
	{
		_list=NULL;
		_capacity=0;
		_auto_shrink = auto_shrink;
		_shrink_trigger = 0;
		_head = 0;
		_tail = 0;
	}

	inline int capacity()
	{
		return _capacity;
	}

	inline int size()
	{
		if (_tail >= _head)
			return _tail - _head;
		else
			return _capacity - (_head - _tail);
	}

	inline T peek(int n)
	{
		bool outof;
		T result  = peek(n, outof);
		if (outof) throw std::exception("peek out of range");
		return result;
	}

	T peek(int n, bool &outof)
	{
		if (_tail >= _head)
		{
			outof = n >= _tail - _head || n<0;
			if (outof)
				return T(0);
			else
				return _list[_head+n];
		}
		else
		{
			outof = n >= _capacity - (_head - _tail) || n<0;
			if (outof)
				return T(0);
			else
			{
				int pos = _head+n;
				if (pos<_capacity && pos>=_head) //pos>=_head: test for integer overflow
					return _list[pos];
				else
					return _list[pos-_capacity];
			}
			return 0;
		}
	}

	void push(T item)
	{
	  // Count+1 because we don't allow situation where entire buffer is full and
      // _head=_tail which is indistinguishable from situation "empty queue"
		if (size() + 1 >= _capacity)
			growshrink(false);
		if (_tail >= _capacity)
			_tail = 0;
		_list[_tail] = item;
		_tail++;
  	}

	T pop()
	{
		if (_head != _tail)
		{
			T result = _list[_head];
			_head++;
			assert(_head <= _capacity);
			if (_head == _capacity)
			{
			   assert(_tail <= _capacity);
			   if (_tail == _capacity)
					_tail = 0;
			   _head = 0;
			}
			if (_auto_shrink && _head != _tail && size() <= _shrink_trigger)
				growshrink(true);
			return result;
	    }
		else throw "Pop from empty stack";
	}
};
}

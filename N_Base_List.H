#ifndef N_BASE_LIST_H
#define N_BASE_LIST_H

///allows smart resizing
namespace ab {
  int grow_capacity(int capacity);
  int compute_shrink_trigger(int old_capacity);
  int capacity_for_size(int size);
}

#endif /*N_BASE_LIST_H*/
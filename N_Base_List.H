#ifndef N_BASE_LIST_H
#define N_BASE_LIST_H

///allows smart resizing
namespace afltk {
  int grow_capacity(int capacity);
  int compute_shrink_trigger(int old_capacity);
}

#endif /*N_BASE_LIST_H*/
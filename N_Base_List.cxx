#include "N_Base_List.H"

namespace afltk {
///return the most significant byte which is set, the same as BSR assembler opcode
///but assembler is not portable
int find_set_bit(unsigned int x)
{
int n;
  if (x==0) return 0;
  n = 0;
  if (x<= 0x0000ffff) { n = n+16; x = x << 16; }
  if (x<= 0x00ffffff) { n = n+8; x = x << 8; }
  if (x<= 0x0fffffff) { n = n+4; x = x << 4; }
  if (x<= 0x3fffffff) { n = n+2; x = x << 2; }
  if (x<= 0x7fffffff) { n = n+1; }
  return 31-n;
}

///grows about 1/4 to 1/7 of curent size, wherin sizes always (except small sizes)
//take values 2^n+k*2^{n-2}, where k=0..3
int grow_capacity(int capacity)
{
  int power_of_two, delta;

  power_of_two = 1 << find_set_bit(capacity);
  if (power_of_two >= 64)
      delta = power_of_two / 4;
  else if (power_of_two >= 8)
      delta = power_of_two / 2;
  else
      delta = 4;
  return capacity + delta;
}

///if list shrinks, new sizes must be the same values as when growing
int compute_shrink_trigger(int old_capacity)
{
  int power_of_two;
  int delta;

  if (old_capacity >= 96)
  {
    power_of_two = 1 << find_set_bit(old_capacity);
    if (old_capacity >= power_of_two / 2 * 3)
        delta = power_of_two / 2;
    else if (old_capacity >= power_of_two / 4 * 5)
        delta = power_of_two / 8 * 3;
    else
        delta = power_of_two / 4;
  }
  else if (old_capacity >= 80)
      delta = 32;
  else if (old_capacity >= 16)
      delta = old_capacity / 2;
  else
      delta = 8;
  if (delta > old_capacity)
    return 0;
  else
    return old_capacity - delta + 1;
}
}

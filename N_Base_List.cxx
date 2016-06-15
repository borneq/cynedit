#include "N_Base_List.H"

namespace ab {
///return the most significant bit which is set, the same as BSR assembler opcode
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


int capacity_for_size_grow(int size)
{
  int power_of_two, part;

  if (size<=4) return 4;
  else if (size<=8) return 8;
  power_of_two = 1 << find_set_bit(size);
  if (power_of_two >= 64)
      part = power_of_two >> 2;
  else if (power_of_two >= 8)
	  part = power_of_two >> 1;
  else
	  part = power_of_two;
  int abovepart = (size-power_of_two) % part;
  if (abovepart==0)
	  return size;
  else
	  return size+(part-abovepart);
}

int capacity_for_size_shrink(int size)
{
	if (size <= 1) return 4;
	else if (size <= 5) return 8;
	int power_of_two, part, size1;
	power_of_two = 1 << find_set_bit(size);
	if (power_of_two >= 64)
		part = power_of_two >> 2;
	else if (power_of_two >= 8)
		part = power_of_two >> 1;
	else
		part = power_of_two;
	int abovepart = (size - power_of_two) % part;
	if (abovepart != 1)
		size1 = size + part;
	else
		size1 = size;
	return capacity_for_size_grow(size1);
}


///if list shrinks, new sizes must be the same values as when growing
int compute_shrink_trigger(int capacity)
{
  int power_of_two;
  int delta;

  if (capacity >= 96)
  {
    power_of_two = 1 << find_set_bit(capacity);
    if (capacity >= power_of_two / 2 * 3)
        delta = power_of_two / 2;
    else if (capacity >= power_of_two / 4 * 5)
        delta = power_of_two / 8 * 3;
    else
        delta = power_of_two / 4;
  }
  else if (capacity >= 80)
      delta = 32;
  else if (capacity >= 16)
      delta = capacity / 2;
  else
      delta = 8;
  if (delta > capacity)
    return 0;
  else
    return capacity - delta + 1;
}
}

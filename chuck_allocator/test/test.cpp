#include <iostream>
#include <string>
#include <vector>
#include "ChunckAllocator.h"

int main() {
  // check allocate and construct
  Allocator<int16_t> a1;
  int16_t* a = a1.allocate(1);
  a1.construct(a, 7);
  std::cout << a[0] << '\n';
  a1.deallocate(a, 1);

  // check  rebind
  decltype(a1)::rebind<int64_t>::other a2;

  // check allocate more than one element
  int64_t* s = a2.allocate(2);
  a2.construct(s, 16);
  a2.construct(s + 1, -32);
  std::cout << s[0] << ' ' << s[1] << '\n';
  a1.destroy(a);

  // use in standart container
  std::vector<int, Allocator<int>> vec;
  vec.push_back(111111);
  vec.push_back(222121324);
  std::cout << vec[0] << ' ' << vec[1] << '\n';

  // check assigmnet
  Allocator<int64_t> a3;
  std::cout << a2.get_counter() << " " << a3.get_counter() << "\n";
  a3 = a2;
  std::cout << a2.get_counter() << " " << a3.get_counter() << "\n";

  // check copy
  Allocator<int64_t> a4(a3);
  std::cout << a4.get_counter() << " " << a3.get_counter() << "\n";

  return 0;
}

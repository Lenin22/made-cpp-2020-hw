#include <functional>
#include <iostream>
#include <cassert>
#include <cmath>
#include "compose.h"

const double EPS = 1e-6;

class Foo {
 public:
  Foo(int& val) : value(val) {}
 public:
  int value;
};

int main() {
  const std::function<bool(int)> a = [](int x) -> bool {
    return x > 10;
  };

  const std::function<int(float)> b = [](float y) -> int {
    return static_cast<int>(y * y);
  };

  const std::function<float(bool)> c = [](bool z) -> float {
    return z ? 3.1f : 3.34f;
  };

  const std::function<float(Foo)> d = [](Foo foo) -> float {
    return static_cast<float>(foo.value) / 10.0;
  };

  const std::function<Foo(int)> e = [](int val) -> Foo {
    return Foo(val);
  };

  {
    auto f = compose(b);
    assert(f(9.8) == 96);
  }

  {
    auto f = compose(a, b, c, a, b, c);
    assert(f(true) == 1);
  }

  {
    auto f = compose(d, e);
    int val = 73;
    assert(fabs(f(val) - 7.3) < EPS);
  }

  {
    try {
      auto f = compose(c, b, a);
    }
    catch (const std::invalid_argument& ex) {
      std::cout << ex.what();
    }
  }

  return 0;
}

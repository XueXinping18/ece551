#include <cassert>

#include "function.h"
class CountedFunctionWrapper : public Function<int, int> {
 protected:
  unsigned remaining;
  Function<int, int> * f;
  const std::string msg;

 public:
  CountedFunctionWrapper(unsigned n, Function<int, int> * fn, const std::string & m) :
      remaining(n), f(fn), msg(m) {}
  virtual int invoke(int arg) override {
    if (remaining == 0) {
      std::cerr << "Too many function invocations: " << msg << std::endl;
      exit(EXIT_FAILURE);
    }
    remaining--;
    return f->invoke(arg);
  }
};
int binarySearchForZero(Function<int, int> * f, int low, int high);
void check(Function<int, int> * f, int low, int high, const std::string & msg) {
  int count = high > low ? log2(high - low) + 1 : 1;
  CountedFunctionWrapper wrapper(count, f, msg);
  // compute correct value by linear scan
  assert(low <= high);
  int expected = low;
  for (int i = low; i < high; i++) {
    if (f->invoke(i) <= 0) {
      expected = i;
    }
    else {
      break;
    }
  }
  int result = binarySearchForZero(&wrapper, low, high);
  if (result != expected) {
    std::cerr << "The result and expected value is different: " << msg << std::endl;
    std::cerr << "expected and result " << expected << " and " << result << std::endl;
    exit(EXIT_FAILURE);
  }
}
class SinFunction : public Function<int, int> {
 public:
  virtual int invoke(int arg) { return 10000000 * (sin(arg / 100000.0) - 0.5); }
};
class IdentityFunction : public Function<int, int> {
 public:
  virtual int invoke(int arg) { return arg; }
};
class PowerFunction : public Function<int, int> {
 public:
  virtual int invoke(int arg) { return 2 * arg * arg * arg - 0.5; }
};
int main() {
  SinFunction sin;
  IdentityFunction identity;
  check(&sin, 0, 150000, "sin 0 150000");
  check(&sin, 31, 31, "sin 31 31");
  check(&sin, 0, 0, "sin 0 0");
  check(&identity, -1, 5, "i -1 5");
  check(&identity, 0, 5, "i 0 5");
  check(&identity, 2, 5, "i 2 5");
  check(&identity, -5, 1, "i -5 1");
  check(&identity, -5, 0, "i -5 0");
  check(&identity, -5, 6, "i 5 6");
  check(&identity, -1, 0, "i -1 -0");
  check(&identity, -5, -5, "i -5 -5");
  check(&identity, 0, 0, "i 0 0");
  check(&identity, -50, 70, "i -50 70");
  check(&identity, -10, -1, "i -10 -1");
  PowerFunction power;
  check(&power, -20, 0, "power -20 0");
  check(&power, -20, 1, "power, -20 1");
  check(&power, -20, -1, "power, -20 -1");
  check(&power, -20, 2, "power, -20 2");
  check(&power, -1, 10, "pwer -1 10");
  check(&power, 0, 10, "pwer 0 10");
  check(&power, 1, 10, "pwer 1 10");
  check(&power, 2, 10, "pwer 2 10");
}

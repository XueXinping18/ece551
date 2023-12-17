#include <iostream>

#include "bstset.h"
using namespace std;
int main() {
  BstSet<int> bstset;
  Set<int> * set = &bstset;
  set->add(0);
  set->add(1);
  set->add(3);
  set->contains(2);
  set->contains(1);
  set->remove(1);
  set->remove(0);
}

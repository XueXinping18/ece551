#include <iostream>

#include "bstmap.h"
using namespace std;
int main() {
  BstMap<int, int> bstmap1, bstmap2;
  Map<int, int> * map = &bstmap1;
  map->add(0, 2);
  map->add(0, 4);
  map->add(1, 6);
  cout << map->lookup(1) << endl;
  cout << map->lookup(0) << endl;
  map->remove(2);
  map->remove(0);
  bstmap2 = bstmap1;
  BstMap<int, int> bstmap3(bstmap1);
}

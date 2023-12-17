#include <assert.h>

#include <cstdlib>
#include <iostream>

#include "il.hpp"

class Tester {
 public:
  // testing for default constructor is done for you
  void testDefCtor() {
    IntList il;
    assert(il.head == NULL);
    assert(il.tail == NULL);
    assert(il.getSize() == 0);
  }
  // example of another method you might want to write
  void testAddFront() {
    // WRITE ME
    IntList list;
    list.addFront(0);
    assert(list[0] == 0);
    assert(list.find(0) == 0);
    assert(list.getSize() == 1);
    assert(list.head != NULL);
    assert(list.tail != NULL);
    assert(list.head == list.tail);
    assert(list.head->next == list.head->prev);
    assert(list.head->prev == NULL);
    assert(list.tail->data == 0);
    for (int i = 0; i < 10; i++) {
      list.addFront(i * i);
    }
    assert(list[0] == 81);
    assert(list[1] == 64);
    assert(list.find(0) == 9);
    assert(list.find(64) == 1);
    assert(list.tail != NULL);
    assert(list.head->next->next->next->next->next ==
           list.tail->prev->prev->prev->prev->prev);
    assert(list.head->prev == NULL);
    assert(list.tail->next == NULL);
  }
  // many more tester methods
  void testAddBack() {
    IntList list;
    list.addBack(1);
    assert(list[0] == 1);
    assert(list.getSize() == 1);
    assert(list.head == list.tail);
    assert(list.head->next == NULL);
    assert(list.tail->prev == NULL);
    list.addBack(2);
    assert(list[0] == 1 && list[1] == 2);
    assert(list.find(1) == 0 && list.find(2) == 1);
    assert(list.head != NULL);
    assert(list.head->data == 1);
    assert(list.tail->data == 2);
    assert(list.tail->next == NULL);
    assert(list.tail->prev == list.head);
    assert(list.getSize() == 2);
  }
  void testCopyConstructor() {
    IntList x;
    x.addFront(1);
    x.addFront(2);
    x.addFront(3);
    IntList y(x);
    assert(y.getSize() == 3);
    assert(y[1] == 2);
    assert(y[0] == 3);
    assert(y[2] == 1);
  }
  void testCopyAssignment() {
    IntList x;
    x.addFront(1);
    x.addFront(2);
    x.addFront(3);
    IntList y;
    y.addFront(0);
    y = x;
    assert(y.getSize() == 3 && x.getSize() == 3);
    assert(y.head->data == 3 && x.head->data == 3);
    assert(y[1] == 2 && x[1] == 2);
    assert(y.tail->data == 1 && x.tail->data == 1);
    assert(x.head != y.head);
    assert(x.tail != y.tail);
  }
  void testRemove() {
    IntList x;
    x.addBack(0);
    x.addBack(1);
    x.addBack(2);
    x.addBack(3);
    assert(!x.remove(100));
    assert(x.remove(1));
    assert(x.getSize() == 3);
    assert(x.head->data == 0 && x.tail->data == 3);
    assert(x.head->next == x.tail->prev);
    assert(x.head->next->next == x.tail);
    assert(x.remove(3));
    assert(x.getSize() == 2);
    assert(x.head->data == 0 && x.tail->data == 2);
    assert(x.head->next == x.tail);
    assert(x.head == x.tail->prev);
    assert(x.remove(0));
    assert(x.getSize() == 1);
    assert(x.head->data == 2 && x.tail->data == 2);
    assert(x.head == x.tail);
    assert(x.head->next == NULL);
    assert(x.tail->prev == NULL);
    assert(x.remove(2));
    assert(x.getSize() == 0);
    assert(x.head == x.tail);
    assert(x.tail == NULL);
    assert(!x.remove(20));
  }
};

int main(void) {
  Tester t;
  t.testDefCtor();
  t.testAddFront();
  t.testAddBack();
  t.testCopyConstructor();
  t.testCopyAssignment();
  t.testRemove();
  // write calls to your other test methods here
  return EXIT_SUCCESS;
}

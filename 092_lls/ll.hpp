#ifndef __LL_HPP__
#define __LL_HPP__

#include <assert.h>

#include <cstdlib>
#include <exception>

//YOUR CODE GOES HERE
class OutOfBoundExpection : public std::exception {};
template<typename T>
class LinkedList {
 private:
  class Node {
   public:
    T data;
    Node * prev;
    Node * next;
    Node(const T & val) : data(val), prev(NULL), next(NULL) {}
    Node(const T & val, Node * prev, Node * next) : data(val), prev(prev), next(next) {}
  };
  Node * head;
  Node * tail;
  int length;

 public:
  LinkedList() : head(NULL), tail(NULL), length(0) {}
  LinkedList(const LinkedList<T> & rhs) : head(NULL), tail(NULL), length(rhs.length) {
    if (length != 0) {
      head = new Node(rhs.head->data);
      Node * currentA = head;
      Node * currentB = rhs.head->next;
      while (currentB != NULL) {
        currentA->next = new Node(currentB->data);
        currentA->next->prev = currentA;
        currentA = currentA->next;
        currentB = currentB->next;
      }
      tail = currentA;
    }
  }
  LinkedList<T> & operator=(const LinkedList<T> & rhs) {
    if (this == &rhs) {
      return *this;
    }
    // create temporary fields in place for the fields in this object
    Node *tmpHead = NULL, *tmpTail = NULL;
    if (length != 0) {
      tmpHead = new Node(rhs.head->data);
      Node * currentA = tmpHead;
      Node * currentB = rhs.head->next;
      while (currentB != NULL) {
        currentA->next = new Node(currentB->data);
        currentA->next->prev = currentA;
        currentA = currentA->next;
        currentB = currentB->next;
      }
      tmpTail = currentA;
    }
    // delete old memory
    Node * current = head;
    while (current != NULL) {
      Node * ptr = current;
      current = current->next;
      delete ptr;
    }
    // swap the fields
    head = tmpHead;
    tail = tmpTail;
    length = rhs.length;

    return *this;
  }
  virtual ~LinkedList() {
    Node * current = head;
    while (current != NULL) {
      Node * ptr = current;
      current = current->next;
      delete ptr;
    }
  }
  int getSize() const { return length; }
  void addFront(const T & item) {
    Node * newNode = new Node(item);
    if (length == 0) {
      head = newNode;
      tail = newNode;
    }
    else {
      // case where there are at least one element;
      newNode->next = head;
      head->prev = newNode;
      head = newNode;
    }
    length++;
  };
  void addBack(const T & item) {
    Node * newNode = new Node(item);
    if (length == 0) {
      head = newNode;
      tail = newNode;
    }
    else {
      newNode->prev = tail;
      tail->next = newNode;
      tail = newNode;
    }
    length++;
  }
  bool remove(const T & item) {
    Node * current = head;
    while (current != NULL) {
      if (current->data == item) {
        // remove the node from the chain
        if (current->prev != NULL) {
          current->prev->next = current->next;
        }
        else {
          // The first element
          head = current->next;
        }
        if (current->next != NULL) {
          current->next->prev = current->prev;
        }
        else {
          // the last element
          tail = current->prev;
        }
        length--;
        delete current;
        return true;
      }
      current = current->next;
    }
    return false;
  }
  T & operator[](int index) {
    if (index >= length || index < 0) {
      throw OutOfBoundExpection();
    }
    Node * current = head;
    for (int i = 0; i < index; i++) {
      current = current->next;
    }
    return current->data;
  }
  const T & operator[](int index) const {
    if (index < 0 || index >= length) {
      throw OutOfBoundExpection();
    }
    Node * current = head;
    for (int i = 0; i < index; i++) {
      current = current->next;
    }
    return current->data;
  }
  int find(const T & item) {
    int i = 0;
    Node * current = head;
    while (current != NULL) {
      if (current->data == item) {
        return i;
      }
      current = current->next;
      i++;
    }
    return -1;
  }
  friend class Tester;
};
#endif

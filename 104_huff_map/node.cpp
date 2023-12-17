#include "node.h"
void Node::buildMap(BitString b, std::map<unsigned, BitString> & theMap) {
  if (this->sym != NO_SYM) {
    theMap[this->sym] = b;
  }
  else {
    this->left->buildMap(b.plusZero(), theMap);
    this->right->buildMap(b.plusOne(), theMap);
  }
}

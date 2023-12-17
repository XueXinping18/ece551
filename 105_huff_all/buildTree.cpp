#include "node.h"

Node * buildTree(uint64_t * counts) {
  priority_queue_t pq;

  // Add nodes for all characters with non-zero frequency
  for (unsigned i = 0; i <= 256; ++i) {
    if (counts[i] != 0) {
      pq.push(new Node(i, counts[i]));
    }
  }

  // Build the tree
  while (pq.size() > 1) {
    Node * left = pq.top();
    pq.pop();
    Node * right = pq.top();
    pq.pop();

    Node * parent = new Node(left, right);
    pq.push(parent);
  }

  // Return the root of the tree
  return pq.top();
}

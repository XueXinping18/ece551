#ifndef __BSTMAP_H__
#define __BSTMAP_H__
#include <iostream>

#include "map.h"
template<typename K, typename V>
class BstMap : public Map<K, V> {
  class TreeNode {
   public:
    K key;
    V val;
    TreeNode * left;
    TreeNode * right;
    TreeNode(const K & key, const V & val) :
        key(key), val(val), left(NULL), right(NULL) {}
    TreeNode(const K & key, const V & val, TreeNode * left, TreeNode * right) :
        key(key), val(val), left(left), right(right) {}
  };

 private:
  TreeNode * root;
  void clear(TreeNode * node) {
    if (node == NULL) {
      return;
    }
    clear(node->left);
    clear(node->right);
    // std::cout << "current node is deleted. key: " << node->key << std::endl;
    delete node;
  }
  TreeNode * copyNode(const TreeNode * node) const {
    if (node == NULL) {
      return NULL;
    }
    return new TreeNode(
        node->key, node->val, copyNode(node->left), copyNode(node->right));
  }
  TreeNode * addHelper(const K & key, const V & val, TreeNode * node) {
    // case where no new node created
    if (node != NULL && key == node->key) {
      node->val = val;
      return node;
    }
    if (node == NULL) {
      // bugs wasting my time: allocate memory on heap outside the current case
      TreeNode * newNode = new TreeNode(key, val);
      return newNode;
    }
    if (key > node->key) {
      // memory should be allocated inside the recursion not outside
      node->right = addHelper(key, val, node->right);
    }
    else if (key < node->key) {
      node->left = addHelper(key, val, node->left);
    }
    return node;
  }
  const V & findHelper(const K & key, TreeNode * node) const {
    if (node == NULL) {
      throw std::invalid_argument("Can't find the val associated with the key.");
    }
    if (node->key == key) {
      return node->val;
    }
    else if (key < node->key) {
      return findHelper(key, node->left);
    }
    else {
      return findHelper(key, node->right);
    }
  }
  TreeNode * removeHelper(const K & key, TreeNode * node) {
    if (node == NULL)
      return NULL;
    if (node->key > key) {
      node->left = removeHelper(key, node->left);
    }
    else if (node->key < key) {
      node->right = removeHelper(key, node->right);
    }
    else {
      // case where search hits
      if (node->left == NULL) {
        TreeNode * tmp = node->right;
        delete node;
        return tmp;
      }
      else if (node->right == NULL) {
        TreeNode * tmp = node->left;
        delete node;
        return tmp;
      }
      else {
        TreeNode * tmp = node->right;
        // case where both childs are not null
        TreeNode * current = node->right;
        while (current->left != NULL) {
          current = current->left;
        }
        current->left = node->left;
        delete node;
        return tmp;
      }
    }
    return node;
  }

 public:
  BstMap() : root(NULL){};
  BstMap(const BstMap<K, V> & rhs) : root(copyNode(rhs.root)) {}
  BstMap & operator=(const BstMap<K, V> & rhs) {
    if (&rhs == this) {
      return *this;
    }
    TreeNode * tmp = copyNode(rhs.root);
    clear(root);
    root = tmp;
    return *this;
  }
  virtual ~BstMap() { clear(root); }
  virtual void add(const K & key, const V & val) { root = addHelper(key, val, root); }
  virtual const V & lookup(const K & key) const throw(std::invalid_argument) {
    return findHelper(key, root);
  }
  virtual void remove(const K & key) { root = removeHelper(key, root); }
};

#endif

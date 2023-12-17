#ifndef __BSTSET_H__
#define __BSTSET_H__
#include "bstmap.h"
#include "set.h"
template<typename T>
class BstSet : public Set<T> {
 private:
  BstMap<T, T> map;

 public:
  virtual void add(const T & key) { map.add(key, key); }
  virtual bool contains(const T & key) const {
    try {
      map.lookup(key);
    }
    catch (std::invalid_argument & e) {
      return false;
    }
    return true;
  };
  virtual void remove(const T & key) { map.remove(key); };
  virtual ~BstSet<T>() {}
};

#endif

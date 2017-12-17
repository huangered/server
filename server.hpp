//
//  btree.hpp
//  kkkk
//
//  Created by peter.huang on 21/11/2017.
//  Copyright © 2017 peter.huang. All rights reserved.
//

#ifndef btree_hpp
#define btree_hpp

#include <iostream>
#include <vector>

namespace linn {

template <class V> class btreedata {
public:
  V value;
  btreedata<V> *left;
  btreedata<V> *right;

  btreedata(V value) : value(value), left(nullptr), right(nullptr) {}
};
    
    template<class ValueType>
    struct btreedataIter : std::iterator<std::bidirectional_iterator_tag, ValueType>{
        
    };
    
template <class K, class V, int _degree> class btreenode {
public:
  bool _leaf;
  std::vector<K> _keys;
  std::vector<btreenode<K, V, _degree> *> _nodes;
  std::vector<btreedata<V> *> _data;
  btreedata<V> *head, *tail;
  void iggg();

public:
  btreenode() { head = tail = nullptr; }
  ~btreenode() {}
  btreenode(const btreenode &n) = delete;
  btreenode &operator=(const btreenode &other) = delete;

  V getMiddleElem() { return _keys[_degree / 2]; }

  void insert(K key, V value) {
    if (_leaf) {
      auto up = std::upper_bound(_keys.begin(), _keys.end(), key);
      int distance = std::distance(_keys.begin(), up);
      _keys.insert(up, key);
      btreedata<V> *d = new btreedata<V>(value);
      if (!_data.empty()) {
        if (distance >= 0 && distance < _data.size()) {
          int index = distance;
          btreedata<V> *first = _data[index];

          d->right = first;
          d->left = first->left;
          if (d->left != nullptr) {
            d->left->right = d;
          }
          first->left = d;
        } else {
          int index = distance - 1;
          btreedata<V> *f = _data[index];
          d->right = f->right;
          d->left = f;
          if (d->right != nullptr) {
            d->right->left = d;
          }
          f->right = d;
        }
      }

      _data.insert(_data.begin() + distance, d);
      return;
    }
    int index = -1;
    for (int i = 0; i < _keys.size(); i++) {
      if (key < _keys[i]) {
        index = i;
        break;
      }
    }
    if (index == -1) {
      index = _keys.size();
    }
    if (_nodes[index]->_keys.size() == _degree) {
      int mid = _nodes[index]->getMiddleElem();
      btreenode<K, V, _degree> *g = _nodes[index]->split();

      _keys.insert(_keys.begin() + index, mid);
      _nodes.insert(_nodes.begin() + index + 1, g);
      if (key < mid) {
        _nodes[index]->insert(key, value);
      } else {
        g->insert(key, value);
      }
    } else {
      _nodes[index]->insert(key, value);
    }
  }
  btreedata<V> *find(int key) {
    if (_leaf) {
      auto iter = std::find(_keys.begin(), _keys.end(), key);
      if (iter != _keys.end()) {
        return _data[std::distance(_keys.begin(), iter)];
      } else {
        return nullptr;
      }
    } else {
      auto iii = std::upper_bound(_keys.begin(), _keys.end(), key);
      int distance = std::distance(_keys.begin(), iii);
      return _nodes[distance]->find(key);
    }
  }
  btreenode *split() {
    btreenode<K, V, _degree> *newNode = new btreenode<K, V, _degree>();
    newNode->_leaf = _leaf;
    int mid = _degree / 2;
    auto b = _keys.begin() + mid;
    newNode->_keys.assign(b, _keys.end());
    _keys.erase(b, _keys.end());
    if (!_leaf) {
      auto c = _nodes.begin() + mid;
      newNode->_nodes.assign(c, _nodes.end());
      _nodes.erase(c, _nodes.end());
    } else {
      auto c = _data.begin() + mid;
      newNode->_data.assign(c, _data.end());
      _data.erase(c, _data.end());
    }
    return newNode;
  }

  btreedata<V> *first() {
    if (_leaf) {
      return *(_data.begin());
    } else {
      return (*(_nodes.begin()))->first();
    }
  }
};
template <class K, class T, int degree = 400> class tree {
public:
  btreenode<K, T, degree> *root;
  btreedata<T> *head, *tail;

  tree() : root(nullptr) { head = tail = nullptr; }
  ~tree();
    
    typedef btreedataIter<btreedata<T>> iterator;
    
    
  void insert(K key, T value);
  btreedata<T> *find(K key);
  iterator *begin();
  btreedata<T> *end();
};
template <class K, class V, int degree>
btreedata<V> *tree<K, V, degree>::find(K key) {
  if (root == nullptr) {
    return nullptr;
  } else {
    return root->find(key);
  }
}
template <class K, class V, int degree>
void tree<K, V, degree>::insert(K key, V value) {
  {
    if (root == nullptr) {
      btreenode<K, V, degree> *node = new btreenode<K, V, degree>();
      node->_leaf = true;
      node->insert(key, value);
      root = node;
    } else {
      if (root->_keys.size() == degree) {
        V mid = root->getMiddleElem();
        btreenode<K, V, degree> *gg = root->split();

        btreenode<K, V, degree> *newRoot = new btreenode<K, V, degree>();
        newRoot->_leaf = false;
        newRoot->_keys.push_back(mid);
        newRoot->_nodes.push_back(root);
        newRoot->_nodes.push_back(gg);
        root = newRoot;
      }
      root->insert(key, value);
    }
  }
}
template <class K, class T, int degree>
  typename  tree<K,T,degree>::iterator *tree<K, T, degree>::begin() {
  return root == nullptr ? nullptr : iterator(root->first());
}
template <class K, class V, int degree>
btreedata<V> *tree<K, V, degree>::end() {
  return nullptr;
}
template <class K, class T, int degree>
tree<K, T, degree>::~tree<K, T, degree>() {
  {
    if (root != nullptr) {
      delete root;
    }
  }
}

} // namespace linn
#endif /* btree_hpp */

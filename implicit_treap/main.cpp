#include <cassert>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

struct Treap;

void deleteTreap(Treap *treap);

struct Treap {
  int key, prio;
  int count = 1;
  int sum;
  Treap *left =
      nullptr; // кланговый форматтер по дефолту делает звёздочки направо, какой
               // кошмар. ещё и коммент этот разбил на две строчки, буе
  Treap *right = nullptr;

  Treap(int key) : key(key), prio(rand()), left(nullptr), right(nullptr) {}
  void toString() {
    cout << "key: " << key << " prio: " << prio << " size: " << count
         << " sum: " << sum << endl;
    if (left) {
      cout << "my(" << key << ")left child: " << endl;
      left->toString();
    }
    if (right) {
      cout << "my(" << key << ")right child: " << endl;
      right->toString();
    }
  }
};

void update(Treap *treap) {
  treap->count = 1;
  treap->sum = treap->key;
  if (treap->left) {
    update(treap->left);
    treap->count += treap->left->count;
    treap->sum += treap->left->sum;
  }
  if (treap->right) {
    update(treap->right);
    treap->count += treap->right->count;
    treap->sum += treap->right->sum;
  }
}

Treap *find(Treap *treap, int key) {
  if (treap == nullptr)
    return nullptr;
  if (treap->key == key)
    return treap;
  if (treap->key >= key)
    return find(treap->left, key);
  return find(treap->right, key);
}

pair<Treap *, Treap *> split(Treap *treap, int key) {
  if (treap == nullptr) {
    return {nullptr, nullptr};
  }
  if (treap->key < key) {
    pair<Treap *, Treap *> pr = split(treap->right, key);
    treap->right = pr.first;
    return {treap, pr.second};
  } else {
    pair<Treap *, Treap *> pr = split(treap->left, key);
    treap->left = pr.second;
    return {pr.first, treap};
  }
}

int getCount(Treap *treap) {
  if (treap == nullptr)
    return 0;
  return treap->count;
}

pair<Treap *, Treap *> splitBySize(Treap *treap, int size) {
  if (treap == nullptr) {
    return {nullptr, nullptr};
  }
  if (size <= getCount(treap->left)) {
    pair<Treap *, Treap *> spl = splitBySize(treap->left, size);
    treap->left = spl.second;
    update(treap);
    return {spl.first, treap};
  }
  pair<Treap *, Treap *> spl =
      splitBySize(treap->right, size - getCount(treap->left) - 1);
  treap->right = spl.first;
  update(treap);
  return {treap, spl.second};
}

Treap *merge(Treap *left, Treap *right) {
  if (left == nullptr) {
    return right;
  }
  if (right == nullptr) {
    return left;
  }
  if (left->prio > right->prio) {
    left->right = merge(left->right, right);
    update(left);
    return left;
  }
  right->left = merge(left, right->left);
  update(right);
  return right;
}

Treap *insert(Treap *treap, int key, int pos) {
  pair<Treap *, Treap *> spl = split(treap, pos);
  Treap *new_treap = new Treap(key);
  return merge(spl.first, merge(new_treap, spl.second));
}

pair<Treap *, int> sum(Treap *treap, int from, int to) {
  pair<Treap *, Treap *> spl = splitBySize(treap, from - 1);
  pair<Treap *, Treap *> spl2 = splitBySize(spl.second, to - from + 1);
  int sum = spl2.first->sum;
  treap = merge(spl.first, merge(spl2.first, spl2.second));
  return {treap, sum};
}

Treap *remove(Treap *treap, int key) {
  pair<Treap *, Treap *> spl = split(treap, key);
  if (spl.second) {
    pair<Treap *, Treap *> spl2 = split(spl.second, key);
    if (spl2.first == nullptr) {
      return merge(spl.first, spl2.second);
    }
    delete spl2.first;
    return merge(spl.first, spl2.second);
  }
  return merge(spl.first, spl.second);
}

Treap *build(vector<int> &vec) {
  if (vec.size() == 0)
    return nullptr;
  Treap *treap = new Treap(vec.at(0));
  for (int i = 1; i < vec.size(); i++) {
    treap = insert(treap, vec.at(i), i);
  }
  return treap;
}

void tests() {
  vector<int> vec;
  for (int i = 0; i < 10; i++) {
    vec.push_back(i);
  }
  Treap *tr = build(vec);
  auto pr = sum(tr, 4, 8);
  tr = pr.first;
  assert(pr.second == 25);
  pr = sum(tr, 5, 8);
  tr = pr.first;
  assert(pr.second == 22);
  pr = sum(tr, 6, 8);
  tr = pr.first;
  assert(pr.second == 18);
}

int main() {
  tests();
  return 0;
}

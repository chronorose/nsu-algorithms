#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

template<typename T>
class FibHeap;

template<typename T>
class Node {
    T val;
    Node* parent;
    Node* child;
    Node* left;
    Node* right;
    bool mark;
    int degree;
    Node(T val) {
        parent = nullptr;
        child = nullptr;
        left = right = this;
        mark = false;
        degree = 0;
    }
    friend class FibHeap<T>;
};

template<typename T>
class FibHeap {
    Node<T>* heap;
    Node<T>* merge(Node<T>* fnode, Node<T>* snode) {
        if (fnode == nullptr) return snode;        
        if (snode == nullptr) return fnode;

        if (fnode->val > snode->val) {
            Node<T>* tmp = fnode;
            fnode = snode;
            snode = tmp;
        }
        Node<T>* f_right = fnode->right;
        Node<T>* s_left = snode->left;
        fnode->right = snode;
        snode->left = fnode;
        f_right->left = s_left;
        s_left->right = f_right;
        return fnode;
    }

    void prepareChildren(Node<T>* node) {
        if (node == nullptr) return;
        Node<T>* child = node->child;
        if (child == nullptr) return;
        Node<T>* tmp = child;
        do {
            tmp->parent = nullptr;
            tmp->mark = false;
            tmp = tmp->right;
        } while (tmp != child);
    }

    Node<T>* consolidate() {
        // vector<Node<T>*> trees;
        unordered_map<int, Node<T>*> trees;
    }

    Node<T>* extract_min_() {
        if (heap == nullptr) return nullptr;
        prepareChildren(heap);
        if (heap->right == heap) {
            heap = heap->child;
        } else {
            heap->right->left = heap->left;
            heap->left->right = heap->right;
            heap = merge(heap->right, heap->child);
        }
        heap = consolidate();
    }

    public:
    FibHeap(): heap(nullptr) {}
    void insert(T val) {
        Node<T>* node = new Node<T>(val);
        heap = merge(heap, node);
    }
    T peek_min() {
        return heap->val;
    }
    T extract_min() {
        T ret = peek_min();
        heap = extract_min_();
    }
};

int main() {}

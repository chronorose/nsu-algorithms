#include <vector>
#include <iostream>

using namespace std;

class Node {
    public:
    int val;
    int degree;
    Node* parent;
    Node* child;
    Node* right;
    Node(int val): val(val) {
        degree = 0;
        parent = nullptr;
        child = nullptr;
        right = nullptr;
    }
};

class BHeap {
    Node* heap;  
    public:
    // for heaps of same sizes
    Node* mergeNodes(Node* fnode, Node* snode) {
        if (fnode == nullptr) return snode;
        if (snode == nullptr) return fnode;
        if (fnode->val > snode->val) {
            Node* tmp = fnode;
            fnode = snode;
            snode = tmp;
        }
        Node* child = fnode->child;
        snode->right = child;
        fnode->child = snode;
        snode->parent = fnode;
        fnode->degree++;
        return fnode;
    }
    Node* mergeHeaps(Node* fheap, Node* sheap) {
        if (fheap == nullptr) return sheap;
        if (sheap == nullptr) return fheap;
        Node* carry = nullptr;
        Node* rheap = nullptr;
        Node* rheap_root = nullptr;
        while (fheap != nullptr && sheap != nullptr) {
            if (fheap->degree == sheap->degree && carry != nullptr && fheap->degree == carry->degree) {
                rheap->right = carry;
                carry = mergeNodes(fheap, sheap);
                rheap = rheap->right;
                fheap = fheap->right;
                sheap = sheap->right;
            } else if (fheap->degree == sheap->degree) {
                carry = mergeNodes(fheap, sheap);
                fheap = fheap->right;
                sheap = sheap->right;
            } else if (carry != nullptr) {
                if (carry->degree == fheap->degree) {
                    carry = mergeNodes(carry, fheap);
                    fheap = fheap->right;
                } else if (carry->degree == sheap->degree) {
                    carry = mergeNodes(carry, sheap);
                    sheap = sheap->right;
                } else {
                    if (rheap == nullptr) {
                        rheap = carry;
                        rheap_root = rheap;
                    } else {
                        rheap->right = carry;
                        rheap = rheap->right;
                    }
                }
            } else if (fheap->degree < sheap->degree) {
                    if (rheap == nullptr) {
                    rheap = fheap;
                    rheap_root = rheap;
                } else {
                    rheap->right = fheap;
                    rheap = rheap->right;
                }
                fheap = fheap->right;
            } else {
                if (rheap == nullptr) {
                    rheap = sheap;
                    rheap_root = rheap;
                } else {
                    rheap->right = sheap;
                    rheap = rheap->right;
                }
                sheap = sheap->right;
            }
        }

        while (fheap != nullptr) {
            if (carry != nullptr) {
                if (carry->degree == fheap->degree) {
                    carry = mergeNodes(carry, fheap);
                    fheap = fheap->right;
                } else {
                    if (rheap == nullptr) {
                        rheap = carry;
                        rheap_root = rheap;
                    } else {
                        rheap->right = carry;
                        rheap = rheap->right;
                    }
                    carry = nullptr;
                }
            } else {
                rheap->right = fheap;
                fheap = fheap->right;
                rheap = rheap->right;
            }
        }

        while (sheap != nullptr) {
            if (carry != nullptr) {
                if (carry->degree == sheap->degree) {
                    carry = mergeNodes(carry, sheap);
                    sheap = sheap->right;
                } else {
                    if (rheap == nullptr) {
                        rheap = carry;
                        rheap_root = rheap;
                    } else {
                        rheap->right = carry;
                        rheap = rheap->right;
                    }
                    carry = nullptr;
                }
            } else {
                rheap->right = sheap;
                sheap = sheap->right;
                rheap = rheap->right;
            }
        }

        if (carry != nullptr) {
            if (rheap == nullptr) {
                rheap = carry;
                rheap_root = rheap;
            }
            else rheap->right = carry;
        }
        return rheap_root;
    }
    void deleteAll(Node* node) {
        while (node != nullptr) {
            deleteAll(node->child);
            Node* tmp = node;
            node = node->right;
            delete node;
        }
    }
    BHeap(): heap(nullptr) {}
    void insert(int val) {
        Node* new_node = new Node(val);
        heap = mergeHeaps(heap, new_node);
    }
    void display() {
        displayChildren(heap);
        cout << endl;
        flush(cout);
    } 
    void displayChildren(Node* node) {
        Node* tmp = node;
        while (tmp != nullptr) {
            cout << tmp->val << " -> ( ";
            displayChildren(tmp->child);
            cout << " ) ";
            tmp = tmp->right;
        }
    }
    // ~BHeap() {
    //     deleteAll(heap);
    // }
};

void tests() {
    BHeap bh;
    for(size_t i = 1; i < 9; i++) {
        bh.insert(i);
    }
    bh.display();
}

int main() {
    tests();
}

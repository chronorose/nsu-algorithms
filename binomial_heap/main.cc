#include <vector>
#include <iostream>
#include <optional>

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
        Node* rheap = new Node(0);
        Node* rheap_root = rheap;
        while (fheap != nullptr && sheap != nullptr) {
            if (fheap->degree == sheap->degree && carry != nullptr && fheap->degree == carry->degree) {
                rheap->right = carry;
                Node* fholder = fheap->right;
                Node* sholder = sheap->right;
                carry = mergeNodes(fheap, sheap);
                rheap = rheap->right;
                fheap = fholder;
                sheap = sholder;
            } else if (fheap->degree == sheap->degree) {
                Node* fholder = fheap->right;
                Node* sholder = sheap->right;
                carry = mergeNodes(fheap, sheap);
                fheap = fholder;
                sheap = sholder;
            } else if (carry != nullptr) {
                if (carry->degree == fheap->degree) {
                    Node* fholder = fheap->right;
                    carry = mergeNodes(carry, fheap);
                    fheap = fholder;
                } else if (carry->degree == sheap->degree) {
                    Node* sholder = sheap->right;
                    carry = mergeNodes(carry, sheap);
                    sheap = sholder;
                } else {
                    rheap->right = carry;
                    rheap = rheap->right;
                }
            } else if (fheap->degree < sheap->degree) {
                rheap->right = fheap;
                rheap = rheap->right;
                fheap = fheap->right;
            } else {
                rheap->right = sheap;
                rheap = rheap->right;
                sheap = sheap->right;
            }
        }
        while (fheap != nullptr) {
            if (carry != nullptr) {
                if (carry->degree == fheap->degree) {
                    Node* fholder = fheap->right;
                    carry = mergeNodes(carry, fheap);
                    fheap = fholder;
                } else {
                    rheap->right = carry;
                    rheap = rheap->right;
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
                    Node* sholder = sheap->right;
                    carry = mergeNodes(carry, sheap);
                    sheap = sholder;
                } else {
                    rheap->right = carry;
                    rheap = rheap->right;
                    carry = nullptr;
                }
            } else {
                rheap->right = sheap;
                sheap = sheap->right;
                rheap = rheap->right;
            }
        }

        if (carry != nullptr) {
            rheap->right = carry;
        }
        rheap = rheap_root;
        rheap_root = rheap->right;
        delete rheap;
        return rheap_root;
    }
    void deleteAll(Node* node) {
        while (node != nullptr) {
            deleteAll(node->child);
            Node* tmp = node;
            node = node->right;
            delete tmp;
        }
    }
    Node* findLeftSibling(Node* node) {
        if (node == nullptr) return nullptr;
        if (node->parent == nullptr) return nullptr;
        if (node->parent->child == node) return nullptr;
        Node* child = node->parent->child;
        while(child->right != node) {
            child = child->right;
        }
        return child;
    }
    // we need to find siblings. also swap things around.
    // holy shit its disgusting lmao :)
    void goUp(Node* node) {
        if (node == nullptr) return;
        if (node->parent == nullptr) return;
        if (node->val < node->parent->val) {
            Node* parent = node->parent;
            int degree = node->parent->degree;
            Node* node_right = node->right;
            Node* tmp = node;
            Node* parent_sibling = findLeftSibling(parent);
            Node* node_sibling = findLeftSibling(node);
            parent->degree = node->degree;
            node->degree = degree;
            node->parent = parent->parent;
            node->child = parent->child;
            if (parent_sibling != nullptr) 
                parent_sibling->right = node;
            node->right = parent->right;
            if (parent->parent != nullptr && parent->parent->child == parent) {
                parent->parent->child = node;
            }
        }
    }
    public:
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

    optional<int> peek_min() {
        if (heap == nullptr) return {};
        Node* tmp = heap;
        int min = tmp->val;
        while (tmp != nullptr) {
            if (tmp->val < min) min = tmp->val;    
            tmp = tmp->right;
        }
        return min;
    }
    
    ~BHeap() {
        deleteAll(heap);
    }
};
// TODO: fix bug with two same values
// 4 of the same values in a row break everything :)
void tests() {
    BHeap bh;
    for(size_t i = 9; i > 6; i--) {
        bh.insert(i);
    }
    // bh.insert(10);
    // bh.insert(10);
    // bh.insert(10);
    // bh.insert(10);
    if (auto minim = bh.peek_min()) {
        cout << *minim << endl;
    }
    bh.display();
}

int main() {
    tests();
}

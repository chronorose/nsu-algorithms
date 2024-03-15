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
    // mergeNodes fheap, sheap
    // 9 8 7 6
    // heap - 9
    // heap - 8 -> 9
    // heap - 7; 8->9
    // fheap: 7; 8->9; sheap: 6
    Node* mergeHeaps(Node* fheap, Node* sheap) {
        if (fheap == nullptr) return sheap;
        if (sheap == nullptr) return fheap;
        Node* carry = nullptr;
        Node* rheap = new Node(0);
        Node* rheap_root = rheap;
        while (fheap != nullptr && sheap != nullptr) {
            if (fheap->degree == sheap->degree && carry != nullptr && fheap->degree == carry->degree) {
                rheap->right = carry;
                carry = mergeNodes(fheap, sheap);
                rheap = rheap->right;
                fheap = fheap->right;
                sheap = sheap->right;
            } else if (fheap->degree == sheap->degree) {
                Node* fholder = fheap->right;
                Node* sholder = sheap->right;
                carry = mergeNodes(fheap, sheap);
                fheap = fholder;
                sheap = sholder;
            } else if (carry != nullptr) {
                if (carry->degree == fheap->degree) {
                    carry = mergeNodes(carry, fheap);
                    fheap = fheap->right;
                } else if (carry->degree == sheap->degree) {
                    carry = mergeNodes(carry, sheap);
                    sheap = sheap->right;
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
            displayChildren(carry);
            cout << endl;
        // displayChildren(rheap_root);
        // cout << endl;
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
                    carry = mergeNodes(carry, sheap);
                    sheap = sheap->right;
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

    int peek_min() {
        // костыль потому что optional в плюсах очень неудобный
        if (heap == nullptr) return 0;
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

void tests() {
    BHeap bh;
    for(size_t i = 9; i > 5; i--) {
        bh.insert(i);
    }
    bh.display();
}

int main() {
    tests();
}

#include <cstdint>
#include <iostream>
#include <type_traits>
#include <vector>
#include <cstdio>

using namespace std;

template<typename T> void swap(T* a, T* b) {
    T* tmp = a;
    a = b;
    b = tmp;
}

class Heap {
    vector<int> heap;
    size_t getMaxChild(size_t i) {
        size_t left = 2 * i + 1;
        size_t right = 2 * i + 2;
        if (left >= heap.size()) return 0;
        if (right >= heap.size()) return left;
        if (heap[left] > heap[right]) return left;
        return right;
    }
    size_t getParent(size_t i) {
        if (i == 0) return 0;
        return (i - 1) / 2;
    }
    bool swapTop(size_t i) {
        return heap[i] > heap[getParent(i)];
    }
    void getUp(size_t i) {
        while(swapTop(i)) {
            size_t parent = getParent(i);
            swap<int>(heap[i], heap[parent]);
            i = parent;
        }
    }
    void getDown(size_t i) {
        size_t child = getMaxChild(i);
        while (child != 0) {
            // cout << heap[child] << "child" <<  endl;
            if (heap[i] < heap[child])
                swap(heap[i], heap[child]);
            i = child;
            child = getMaxChild(i);
        }
    }
    public:
    Heap(int x) {
        heap.push_back(x);
    }
    void insert(int x) {
        heap.push_back(x);
        getUp(heap.size() - 1);
    }
    void display() {
        for (size_t i = 0; i < heap.size(); i++) {
            cout << heap[i] << " ";
        }
        cout << endl;
    }
    int getMax() {
        int ret = heap[0];
        swap<int>(heap[0], heap[heap.size() - 1]);   
        heap.pop_back();
        getDown(0);
        // this->display();
        return ret;
    }
};

int main() {
    Heap heap(0);
    heap.insert(2);
    heap.insert(4);
    heap.insert(5);
    heap.insert(8);
    heap.display();
    cout << heap.getMax() << endl;
    heap.display();
    cout << heap.getMax() << endl;
    heap.display();
    cout << heap.getMax() << endl;
}

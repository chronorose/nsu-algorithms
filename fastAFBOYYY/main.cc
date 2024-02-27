#include <iostream>
#include <vector>

using namespace std;

void swap(int& a, int& b) {
    int tmp = a;
    a = b;
    b = tmp;
}

int partition(vector<int>& vec, size_t left, size_t right) {
    int pivot = vec[left]; 
    int i = left + 1;
    for (size_t j = left + 1; j <= right; j++) {
        if (vec[j] <= pivot) {
            swap(vec[i++], vec[j]);
        }
    }
    swap(vec[i - 1], vec[left]);
    return i - 1;
}

void quicksort(vector<int>& vec, size_t left, size_t right) {
    if (left < right) {

        size_t new_pivot = partition(vec, left, right);
        quicksort(vec, left, new_pivot - 1);
        quicksort(vec, new_pivot + 1, right);
    }
}

void printVec(vector<int>& vec) {
    for (size_t i = 0; i < vec.size(); i++) {
        cout << vec[i] << " ";
    }
    cout << endl;
}

int main() {
    vector<int> a = {4, 1, 7, 8, 3, 33, 2, 12};
    quicksort(a, 0, a.size() - 1);
    printVec(a);
}

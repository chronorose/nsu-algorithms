#include <iostream>
#include <vector>

using namespace std;

void swap(int& a, int& b) {
    int tmp = a;
    a = b;
    b = tmp;
}

void sort_colors(vector<int>& vec) {
    size_t i = 0, j = vec.size() - 1, k = i + 1;
    while(k < j) {
        cout << "i: " << i << "; k: " << k << "; j: " << j << endl;
        if (vec[k] == 0 && k > i) {
            swap(vec[i++], vec[k]);
            continue;
        }
        if (vec[k] == 2) {
            swap(vec[j--], vec[k]);
            continue;
        }
        k++;
    }
}

void printVec(vector<int>& vec) {
    for (size_t i = 0; i < vec.size(); i++) {
        cout << vec[i] << " ";
    }
    cout << endl;
}

int main() {
    vector<int> vec = {2, 0, 2, 1, 1, 0};
    sort_colors(vec);
    printVec(vec);
}

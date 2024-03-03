#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

void swap(int& a, int& b) {
    int tmp = a;
    a = b;
    b = tmp;
}

int partition(vector<int>& vec, int left, int right) {
    if (left >= right) return -1;
    swap(vec[left], vec[left + rand() % (right - left + 1)]);
    int pivot = left;
    int i = pivot + 1;
    int j = right;
    while (i <= j) {
        if (vec[i] >= vec[pivot] && vec[j] < vec[pivot]) swap(vec[i], vec[j]);
        else if(vec[j] >= vec[pivot]) j--;
        else i++;
    }
    swap(vec[pivot], vec[j]);
    return j;
}

int kth(vector<int>& vec, int left, int right, int k) {
    if (right <= left) {
        if (left == right) return vec[left];
        if (left > right) return -1;
    }
    int p = partition(vec, left, right);
    if (p + 1 == k) {
        return vec[p];
    } else if (p + 1 > k) {
        return kth(vec, left, p - 1, k);
    } else {
        return kth(vec, p + 1, right, k);
    }
}

int kth(vector<int>&vec, int k) {
    if (k > vec.size()) return -1;
    if (vec.size() == 2) {
        int p = partition(vec, 0, vec.size() - 1);
        if (p == k - 1) return vec[p];
        else return vec[1 - p];
    }
    return kth(vec, 0, vec.size() - 1, k);
}

// так как учитывать расположение вышек по иксу бесполезно в контексте задачи, пусть местоположение вышек
// это массив интов, которые отвечают за их положение по y.
// у чётных вроде как неважно брать ли (size / 2) + 1 или (size / 2) + 2 или значение посередине между ними.

int neft(vector<int>& vec) {
    return kth(vec, vec.size() / 2 + 1);
}

void tests() {
    vector<int> vec = {1, 3, 6};
    assert(neft(vec) == 3);
    vec = {1, 2, 4, 9};
    assert(neft(vec) == 4);
}

int main() {
    tests();
}

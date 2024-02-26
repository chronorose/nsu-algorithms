#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>

using namespace std;

size_t getLen(vector<string>& vec) {
    size_t max = vec.at(0).size();
    for (size_t i = 1; i < vec.size(); i++) {
        if (vec.at(i).size() > max) max = vec.at(i).size();
    }
    return max;
}

void countSort(vector<string>& vec, int sym) {
    vector<string> buf(vec.size());
    size_t size = 256;
    vector<int> count(size, 0);
    for (size_t i = 0; i < vec.size(); i++) {
        count[vec[i][sym]]++;
    }
    for (size_t i = 1; i < size - 1; i++) {
        count[i] += count[i - 1];
    }
    for(int i = vec.size() - 1; i >= 0; i--) {
        buf[--count[vec[i][sym]]] = vec[i];
    }
    for(size_t i = 0; i < vec.size(); i++) {
        vec[i] = buf[i];
    }
}

void radix(vector<string>& vec) {
    size_t ln = getLen(vec);
    for (int i = ln - 1; i >= 0; i--) {
        countSort(vec, i);
    }
}

string generateRandomString(size_t len) {
    string a;
    for (size_t i = 0; i < len; i++) {
        a.push_back(rand() % ('z' - 'a' + 1) + 'a');
    }
    return a;
}

vector<string> generateRandomVec(size_t lenVec, size_t lenStr) {
    vector<string> a;
    for(size_t i = 0; i < lenVec; i++) {
        a.push_back(generateRandomString(lenStr));
    }
    return a;
}

int main() {
    vector<string> vec;
    srand(time(0));
    for (size_t i = 0; i < 10; i++) {
        vec.push_back(generateRandomString(10));
    }
    vector<string> vec1 = vec;
    sort(vec1.begin(), vec1.end());
    for (size_t i = 0; i < vec.size(); i++) {
        cout << vec[i] << endl;
    }
    cout << "sorted: " << endl;
    for (size_t i = 0; i < vec1.size(); i++) {
        cout << vec1[i] << endl;
    }
    radix(vec);
    cout << "sorted via radix: " << endl;
    for (size_t i = 0; i < vec.size(); i++) {
        cout << vec[i] << endl;
    }
}

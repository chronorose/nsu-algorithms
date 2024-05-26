#include <iostream>
#include <cmath>
#include "bloom_filter.hpp"

using namespace std;

ostream& operator<<(ostream& os, const Ip& ip) {
    for (size_t i = 0; i < 3; i++) {
        os << (int)ip.ip[i] << ".";
    }
    os << (int)ip.ip[3];
    return os;
}
ostream& operator<<(ostream& os, const BloomFilter& bf) {
    for (size_t i = 0; i < bf.bitset.size(); i++) {
        os << bf.bitset[i];
    }
    return os;
}

void test() {
    cout << "please enter amount of elements estimated: ";    
    size_t amount;
    cin >> amount;
    cout << "please enter precision: ";
    double precision;
    cin >> precision;
    srand(time(0));
    unordered_set<string> unique_ones;
    vector<Ip> vec(amount);
    for (size_t i = 0; i < amount; ++i) {
        do {
            Ip ip;
            if (unique_ones.contains(ip.to_string())) continue;
            unique_ones.insert(ip.to_string());
            vec[i] = ip;
        } while(0);
    }
    BloomFilter bf(precision, amount);
    for (size_t i = 0; i < amount; i++) {
        bf.insert(vec[i]);
        cout << "-------------------------------------" << endl;
        cout << "After inserting: ";
        cout << vec[i] << " Bloom filter looks as follows: " << endl;
        cout << bf << endl << endl;
    }
    cout << "FP: " << bf.collisions << endl;
    cout << "FP rate: " << (double)bf.collisions / (double) amount << endl;
}

int main() {
    test();
}

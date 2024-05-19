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
    BloomFilter bf(precision, amount);
    for (size_t i = 0; i < amount; i++) {
        Ip ip;
        bf.insert(ip);
        cout << "-------------------------------------" << endl;
        cout << "After inserting: ";
        cout << ip << " Bloom filter looks as follows: " << endl;
        cout << bf << endl << endl;
    }
}

int main() {
    test();
}

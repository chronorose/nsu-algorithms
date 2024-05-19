#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

const double ln2 = 0.69314718056;
const double logbasehalf = 1 / log(0.5);
const double revln2 = 1 / ln2;

struct Ip {
    unsigned char ip[4];
    Ip() {
        for (size_t i = 0; i < 4; i++) {
            ip[i] = rand() % 257;
        }
    }
};

struct BloomFilter {
    vector<bool> bitset;
    vector<unsigned char> hfunctions;
    size_t haship(Ip ip, unsigned char hf) {
        size_t res = 0;
        for (size_t i = 0; i < 4; i++) {
            res += ip.ip[i] * hf;
        }
        return res % bitset.size();
    }
    BloomFilter(double error, size_t amount) {
        cout << "Creating bloom filter..." << endl;
        double eps = logbasehalf * log(error);
        size_t bitset_size = amount * ceil(eps);
        size_t hashfunction_amount = ceil(eps * revln2);
        cout << "Hash functions amount: " << hashfunction_amount << endl;
        cout << "Bitset size: " << bitset_size << endl;

        bitset.resize(bitset_size, false);
        for (size_t i = 0; i < hashfunction_amount; i++) {
            hfunctions.push_back(rand());
        }
    }
    void insert(Ip ip) {
        for (size_t i = 0; i < hfunctions.size(); ++i) {
            bitset[haship(ip, hfunctions[i])] = true;
        }
    }
    bool lookup(Ip ip) {
        for (size_t i = 0; i < hfunctions.size(); ++i) {
            if (!bitset[haship(ip, hfunctions[i])]) return false;
        }        
        return true;
    }
};

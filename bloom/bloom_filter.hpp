#include <vector>
#include <cmath>

using namespace std;

const double ln2 = 0.69314718056;
const double logbasehalf = 1 / log(0.5);
const double revln2 = 1 / ln2;

class BloomFilter {
    vector<bool> bitset;
    vector<unsigned char> hfunctions;
    BloomFilter(double error, size_t amount) {
        double eps = logbasehalf * log(error);
        size_t bitset_size = amount * ceil(eps);
        size_t hashfunction_amount = ceil(eps * revln2);
        bitset.resize(bitset_size, false);
        for (size_t i = 0; i < hashfunction_amount; i++) {
            hfunctions.push_back(rand() & 0xFF);
        }
    }
    size_t haship(int ip, unsigned char hf) {
        size_t res = 0;
        for (size_t i = 0; i < 4; i++) {
            res += (ip & 0xF) + (hf & 0x3);
            ip >>= 4;
            hf >>= 2;
        }
        return res % bitset.size();
    }
    void insert(int ip) {
        for (size_t i = 0; i < hfunctions.size(); ++i) {
            bitset[haship(ip, hfunctions[i])] = true;
        }
    }
    bool lookup(int ip) {
        for (size_t i = 0; i < hfunctions.size(); ++i) {
            if (!bitset[haship(ip, hfunctions[i])]) return false;
        }        
        return true;
    }
};

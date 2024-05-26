#include <vector>
#include <cmath>
#include <iostream>
#include <unordered_set>
#include <sstream>

using namespace std;

const double ln2 = 0.69314718056;
const double logbasehalf = 1 / log(0.5);
const double revln2 = 1 / ln2;

struct Ip {
    unsigned char ip[4];
    Ip() {
        for (size_t i = 0; i < 4; i++) {
            ip[i] = rand() % 256;
        }
    }
    Ip(vector<int>& vec) {
        for (size_t i = 0; i < 4; i++) {
            ip[i] = vec[i] % 256;
        }
    }
    bool operator<(const Ip& other) {
        for (size_t i = 0; i < 4; i++) {
            if (other.ip[i] > ip[i]) return true;
            else if (other.ip[i] == ip[i]) continue;
            return false;
        }
        return false;
    }
    bool operator<=(const Ip& other) {
        for (size_t i = 0; i < 4; i++) {
            if (other.ip[i] > ip[i]) return true;
            else if (other.ip[i] == ip[i]) continue;
            return false;
        }
        return true;
    }
    bool operator>(const Ip& other) {
        for (size_t i = 0; i < 4; i++) {
            if (other.ip[i] < ip[i]) return true;
            else if (other.ip[i] == ip[i]) continue;
            return false;
        }
        return false;
    }
    bool operator>=(const Ip& other) {
        for (size_t i = 0; i < 4; i++) {
            if (other.ip[i] < ip[i]) return true;
            else if (other.ip[i] == ip[i]) continue;
            return false;
        }
        return true;
    }
    string to_string() {
        stringstream ss;
        for (size_t i = 0; i < 4; i++) {
            ss << ip[i];
        }
        return ss.str();
    }
};

struct BloomFilter {
    vector<bool> bitset;
    vector<unsigned char> hfunctions;
    unordered_set<string> correct_ones {};
    size_t collisions = 0;
    size_t haship(Ip& ip, unsigned char hf) {
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
    void insert(Ip& ip) {
        if (lookup(ip)) ++collisions;
        for (size_t i = 0; i < hfunctions.size(); ++i) {
            bitset[haship(ip, hfunctions[i])] = true;
        }
        correct_ones.insert(ip.to_string());
    }
    bool lookup(Ip& ip) {
        for (size_t i = 0; i < hfunctions.size(); ++i) {
            if (!bitset[haship(ip, hfunctions[i])]) return false;
        }        
        return true;
    }
};

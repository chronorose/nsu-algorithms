#include <iostream>
#include <vector>
#include <cstdlib>
#include <time.h>

using namespace std;

class Matrix;
ostream& operator<<(ostream& os, Matrix mx); 

size_t get2Pow(size_t num) {
    size_t counter = 1;
    if (num != 0)
        num -= 1;
    while (num != 0) {
        num /= 2;
        counter *= 2;
    }
    return counter;
}

class Matrix {
    size_t m;
    size_t n;
    public:
    vector<vector<int>> mat;
    Matrix(size_t n): Matrix(n, n) {} 
    Matrix(size_t m, size_t n) {
        this->m = m;
        this->n = n;
        this->mat = vector<vector<int>>(this->m);
        for(size_t i = 0; i < this->m; i++) {
            this->mat.at(i) = vector<int>(this->n);
        }
    }
    size_t getN() {
        return this->n;
    }
    size_t getM() {
        return this->m;
    }
    int getIJ(size_t i, size_t j) {
        return this->mat.at(i).at(j);
    }
    void setIJ(int num, size_t i, size_t j) {
        this->mat.at(i).at(j) = num;
    }
    void fill() {
        for(size_t i = 0; i < this->m; i++) {
            for(size_t j = 0; j < this->n; j++)
                this->mat.at(i).at(j) = rand() % 100;
        }
    }
    Matrix copyIJ(size_t starti, size_t endi, size_t startj, size_t endj) {
        Matrix newM(endi - starti, endj - startj);
        for (size_t i = starti, copyi = 0; i < endi; i++, copyi++) {
            for (size_t j = startj, copyj = 0; j < endj; j++, copyj++) {
                newM.setIJ(this->getIJ(i, j), copyi, copyj);
            }
        }
        return newM;
    }
    void copyInto(Matrix to_copy, size_t starti, size_t endi, size_t startj, size_t endj) {
        for (size_t i = starti, copyi = 0; i < endi; i++, copyi++) {
            for(size_t j = startj, copyj = 0; j < endj; j++, copyj++) {
                this->setIJ(to_copy.getIJ(copyi, copyj), i, j);
            }
        } 
    }
    pair<size_t, size_t> fill0() {
        size_t newN = get2Pow(this->n);
        size_t newM = get2Pow(this->m);
        for(size_t i = 0; i < this->m; i++) {
            for (size_t j = this->n; j < newN; j++) {
                this->mat.at(i).push_back(0);
            }
        }
        for(size_t i = this->m; i < newM; i++) {
            vector<int> vec(newN);
            vec.insert(vec.begin(), newN, 0);
            this->mat.push_back(vec);
            // this->mat.at(i) = vector<int>(newN);
            // this->mat.at(i).insert(this->mat.at(i).begin(), newN, 0);
        }
        pair<size_t, size_t> pr(newM - this->m, newN - this->n);
        this->m = newM;
        this->n = newN;
        return pr;
    }
    void erase0(pair<size_t, size_t> pr) {
        for (size_t i = 0; i < pr.first; i++)
            this->mat.pop_back();
        this->m = this->m - pr.first;
        this->n = this->n - pr.second;
        for (size_t i = 0; i < this->m; i++) {
            for (size_t j = 0; j < pr.second; j++) {
                this->mat.at(i).pop_back();
            }
        }
    }
    Matrix operator*(Matrix& m2) {
        Matrix newM(this->m, m2.n);
        for (size_t i = 0; i < this->m; i++) {
            for (size_t j = 0; j < m2.n; j++) {
                int product = 0;
                for (size_t k = 0; k < this->n; k++) {
                    product += this->getIJ(i, k) * m2.getIJ(k, j);
                }
                newM.setIJ(product, i, j);
            }
        }
        return newM;
    }
    Matrix operator+(Matrix& m2) {
        Matrix newM(this->m, this->n);
        for (size_t i = 0; i < this->m; i++) {
            for (size_t j = 0; j < this->n; j++) {
                newM.setIJ(this->getIJ(i, j) + m2.getIJ(i, j), i, j);
            }
        }
        return newM;
    }
    Matrix operator-(Matrix& m2) {
        Matrix newM(this->m, this->n);
        for (size_t i = 0; i < this->m; i++) {
            for (size_t j = 0; j < this->n; j++) {
                newM.setIJ(this->getIJ(i, j) - m2.getIJ(i, j), i, j);
            }
        }
        return newM;
    }
    bool operator==(Matrix& m2) {
        if (this->m != m2.m || this->n != m2.n) {
            return false;
        }
        for (size_t i = 0; this->m; i++) {
            for (size_t j = 0; this->n; j++) {
                if (this->getIJ(i, j) != m2.getIJ(i, j)) {
                    return false;
                }
            }
        }
        return true;
    }
};

ostream& operator<<(ostream& os, Matrix mx) {
    for(size_t i = 0; i < mx.getM(); i++) {
        for(size_t j = 0; j < mx.getN(); j++) {
            cout << mx.mat.at(i).at(j) << " ";
        }
        cout << endl;
    }
    return os;
}

Matrix recursiveOne(Matrix& m1, Matrix& m2) {
    if (m1.getM() <= 3 || m1.getN() <= 3 ||
        m2.getM() <= 3 || m2.getN() <= 3)
        return m1 * m2;
    size_t halfN = m1.getN() / 2;
    size_t halfM = m1.getM() / 2;
    Matrix A = m1.copyIJ(0, halfM, 0, halfN);
    Matrix B = m1.copyIJ(0, halfM, halfN, m1.getN());
    Matrix C = m1.copyIJ(halfM, m1.getM(), 0, halfN);
    Matrix D = m1.copyIJ(halfM, m1.getM(), halfN, m1.getN());
    Matrix E = m2.copyIJ(0, halfM, 0, halfN);
    Matrix F = m2.copyIJ(0, halfM, halfN, m2.getN());
    Matrix G = m2.copyIJ(halfM, m2.getM(), 0, halfN);
    Matrix H = m2.copyIJ(halfM, m2.getM(), halfN, m2.getN());

    Matrix AE = recursiveOne(A, E);
    Matrix BG = recursiveOne(B, G);
    Matrix AF = recursiveOne(A, F);
    Matrix BH = recursiveOne(B, H);
    Matrix CE = recursiveOne(C, E);
    Matrix DG = recursiveOne(D, G);
    Matrix CF = recursiveOne(C, F);
    Matrix DH = recursiveOne(D, H);

    Matrix m(m1.getM(), m1.getN());

    m.copyInto(AE + BG, 0, halfM, 0, halfN);
    m.copyInto(AF + BH, 0, halfM, halfN, m1.getN());
    m.copyInto(CE + DG, halfM, m1.getM(), 0, halfN);
    m.copyInto(CF + DH, halfM, m1.getM(), halfN, m1.getN());
    return m;
}

Matrix strassen(Matrix& m1, Matrix& m2) {
    if (m1.getM() <= 3 || m1.getN() <= 3 ||
        m2.getM() <= 3 || m2.getN() <= 3)
        return m1 * m2;
    size_t halfN = m1.getN() / 2;
    size_t halfM = m1.getM() / 2;

    Matrix A = m1.copyIJ(0, halfM, 0, halfN);
    Matrix B = m1.copyIJ(0, halfM, halfN, m1.getN());
    Matrix C = m1.copyIJ(halfM, m1.getM(), 0, halfN);
    Matrix D = m1.copyIJ(halfM, m1.getM(), halfN, m1.getN());
    Matrix E = m2.copyIJ(0, halfM, 0, halfN);
    Matrix F = m2.copyIJ(0, halfM, halfN, m2.getN());
    Matrix G = m2.copyIJ(halfM, m2.getM(), 0, halfN);
    Matrix H = m2.copyIJ(halfM, m2.getM(), halfN, m2.getN());

    Matrix FH = F - H;
    Matrix AB = A + B;
    Matrix CD = C + D;
    Matrix GE = G - E;
    Matrix AD =  A + D;
    Matrix EH = E + H;
    Matrix BD = B - D;
    Matrix GH = G + H;
    Matrix AC = A - C;
    Matrix EF = E + F;
    
    Matrix P1 = strassen(A, FH);
    Matrix P2 = strassen(AB, H);
    Matrix P3 = strassen(CD, E);
    Matrix P4 = strassen(D, GE);
    Matrix P5 = strassen(AD, EH);
    Matrix P6 = strassen(BD, GH);
    Matrix P7 = strassen(AC, EF);

    Matrix m(m1.getM(), m1.getN());

    m.copyInto(P5 + P4 - P2 + P6, 0, halfM, 0, halfN);
    m.copyInto(P1 + P2, 0, halfM, halfN, m1.getN());
    m.copyInto(P3 + P4, halfM, m1.getM(), 0, halfN);
    m.copyInto(P1 + P5 - P3 - P7, halfM, m1.getM(), halfN, m1.getN());
    
    return m;
}

Matrix recursiveWrap(Matrix& m1, Matrix& m2) {
    auto pr1 = m1.fill0();
    auto pr2 = m2.fill0();
    Matrix ret = recursiveOne(m1, m2);
    m1.erase0(pr1);    
    m2.erase0(pr2);
    pair<size_t, size_t> ret_pr(pr1.first, pr2.second);
    ret.erase0(ret_pr);
    return ret;
}

Matrix strassenWrap(Matrix& m1, Matrix& m2) {
    auto pr1 = m1.fill0();
    auto pr2 = m2.fill0();
    Matrix ret = strassen(m1, m2);
    m1.erase0(pr1);    
    m2.erase0(pr2);
    pair<size_t, size_t> ret_pr(pr1.first, pr2.second);
    ret.erase0(ret_pr);
    return ret;
}

int main() {
    srand(time(0));
    Matrix m1(40);  
    Matrix m2(40);
    m1.fill();
    m2.fill();
    cout << m1 * m2 << "strassen: \n" << strassenWrap(m1, m2);
}

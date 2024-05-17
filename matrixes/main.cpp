#include <ctime>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <functional>
#include <cmath>
#include <iomanip>

using namespace std;

#define COLUMN 6

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

    Matrix A = Matrix(halfM, halfN);
    Matrix B = Matrix(halfM, halfN);
    Matrix C = Matrix(halfM, halfN);
    Matrix D = Matrix(halfM, halfN);
    Matrix E = Matrix(halfM, halfN);
    Matrix F = Matrix(halfM, halfN);
    Matrix G = Matrix(halfM, halfN);
    Matrix H = Matrix(halfM, halfN);

    for (size_t i = 0; i < halfM; i++) {
        for (size_t j = 0; j < halfN; j++) {
            A.mat[i][j] = m1.mat[i][j];
            B.mat[i][j] = m1.mat[i][j + halfN];
            C.mat[i][j] = m1.mat[i + halfM][j];
            D.mat[i][j] = m1.mat[i + halfM][j + halfN];
            E.mat[i][j] = m2.mat[i][j];
            F.mat[i][j] = m2.mat[i][j + halfN];
            G.mat[i][j] = m2.mat[i + halfM][j];
            H.mat[i][j] = m2.mat[i + halfM][j + halfN];
        }
    }
    

    Matrix AE = recursiveOne(A, E);
    Matrix BG = recursiveOne(B, G);
    Matrix AF = recursiveOne(A, F);
    Matrix BH = recursiveOne(B, H);
    Matrix CE = recursiveOne(C, E);
    Matrix DG = recursiveOne(D, G);
    Matrix CF = recursiveOne(C, F);
    Matrix DH = recursiveOne(D, H);

    Matrix m(m1.getM(), m1.getN());

    AE = AE + BG;
    AF = AF + BH;
    CE = CE + DG;
    CF = CF + DH;

    for (size_t i = 0; i < halfM; i++) {
        for (size_t j = 0; j < halfN; j++) {
            m.mat[i][j] = AE.mat[i][j];
            m.mat[i][j + halfN] = AF.mat[i][j];
            m.mat[i + halfM][j] = CE.mat[i][j];
            m.mat[i + halfM][j + halfN] = CF.mat[i][j];
        }
    }
    return m;
}

Matrix strassen(Matrix& m1, Matrix& m2) {
    if (m1.getM() <= 16 || m1.getN() <= 16 ||
        m2.getM() <= 16 || m2.getN() <= 16)
        return m1 * m2;
    size_t halfN = m1.getN() / 2;
    size_t halfM = m1.getM() / 2;

    Matrix A = Matrix(halfM, halfN);
    Matrix B = Matrix(halfM, halfN);
    Matrix C = Matrix(halfM, halfN);
    Matrix D = Matrix(halfM, halfN);
    Matrix E = Matrix(halfM, halfN);
    Matrix F = Matrix(halfM, halfN);
    Matrix G = Matrix(halfM, halfN);
    Matrix H = Matrix(halfM, halfN);

    for (size_t i = 0; i < halfM; i++) {
        for (size_t j = 0; j < halfN; j++) {
            A.mat[i][j] = m1.mat[i][j];
            B.mat[i][j] = m1.mat[i][j + halfN];
            C.mat[i][j] = m1.mat[i + halfM][j];
            D.mat[i][j] = m1.mat[i + halfM][j + halfN];
            E.mat[i][j] = m2.mat[i][j];
            F.mat[i][j] = m2.mat[i][j + halfN];
            G.mat[i][j] = m2.mat[i + halfM][j];
            H.mat[i][j] = m2.mat[i + halfM][j + halfN];
        }
    }

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

    P6 = P5 + P4 - P2 + P6;
    P2 = P1 + P2; 
    P4 = P3 + P4;
    P1 = P1 + P5 - P3 - P7;

    for (size_t i = 0; i < halfM; i++) {
        for (size_t j = 0; j < halfN; j++) {
            m.mat[i][j] = P6.mat[i][j];
            m.mat[i][j + halfN] = P2.mat[i][j];
            m.mat[i + halfM][j] = P4.mat[i][j];
            m.mat[i + halfM][j + halfN] = P1.mat[i][j];
        }
    }
    
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

Matrix methodForwarder(Matrix& m1, Matrix& m2) {
    return m1 * m2;
}

float timeMatrixMul(function<Matrix(Matrix&, Matrix&)> func, size_t size) {
    Matrix m1(size); 
    m1.fill();
    Matrix m2(size); 
    m2.fill();
    auto time = clock();
    func(m1, m2);
    time = clock() - time;
    return (float)time / CLOCKS_PER_SEC;
}

template<typename T> T sampleMean(vector<T>& vec) {
    T holder = vec[0];
    for (size_t i = 1; i < vec.size(); i++) {
        holder += vec[i];
    }
    return (holder / vec.size());
}

template<typename T> T geometricMean(vector<T>& vec) {
    auto holder = vec[0] * 1;
    for (size_t i = 1; i < vec.size(); i++) {
        holder  = holder * vec[i];
    }
    holder = pow(holder, 1. / vec.size());
    return holder;
}

template<typename T> T standardDeviation(vector<T>& vec) {
    T mean = sampleMean(vec);
    T holder = vec[0];
    for (size_t i = 1; i < vec.size(); i++) {
        holder += pow(vec[i] - mean, 2);
    }
    holder = holder / vec.size();
    return sqrt(holder);
}


void bench(function<Matrix(Matrix&, Matrix&)> func, const char* name) {
    cout << "-------------------------------------------" << endl;
    cout << "tests for " << name << ": " << endl;
    vector<float> defaultMul;
    for (size_t i = 0; i < 5; i++) {
        srand(time(0));
        auto time = timeMatrixMul(func, 100);
        defaultMul.push_back(time);
        // cout << time << endl;
    }
    auto mean = sampleMean(defaultMul);
    auto geometric_mean = geometricMean(defaultMul);
    auto standardDev = standardDeviation(defaultMul);
    cout << setprecision(6) << "mean: " << mean << endl << "geomean: " << geometric_mean << endl;
    cout << setprecision(6) << "SD: " << standardDev << endl;
    cout << name << " tests ended." << endl;
}

void tests() {
    bench(methodForwarder, "default matrix multiplication");
    bench(strassenWrap, "strassen algorithm");
    bench(recursiveWrap, "recursive algorithm");
}

void show_strassen_superiority() {
    cout << "-------------------------------------------" << endl;
    cout << "now we show when strassen is better" << endl;
    auto time = timeMatrixMul(strassenWrap, 1000);
    cout << "that was strassen: " << time << endl;
    time = timeMatrixMul(methodForwarder, 1000);
    cout << "well that was default algorithm: " << time << endl;
    cout << "-------------------------------------------" << endl;
}

int main() {
    tests();
    show_strassen_superiority();
}

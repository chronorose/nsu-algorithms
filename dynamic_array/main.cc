#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <string.h>

template<typename T> class Vector {
    T* vec;
    size_t size;
    size_t capacity;
    void resize(size_t new_capacity) {
        T* new_vec = new T[new_capacity];
        memmove(new_vec, this->vec, this->size * sizeof(T));
        delete[] this->vec;
        this->vec = new_vec;
    }
    public:
    Vector(): Vector(8) {}
    Vector(size_t capacity) {
        this->vec = new T[capacity];
        this->size = 0;
        this->capacity = capacity;
    }
    void push_back(T data) {
        if (this->size + 1 >= capacity) {
            this->capacity = this->capacity * 2;
            this->resize(this->capacity);
        }
        this->vec[this->size++] = data;
    }
    T at(size_t n) {
        if (n > this->size) {
            return T{}*0;
        }
        return this->vec[n];
    }
    void pop_back() {
        if (this->size == 0) {
            return;
        }
        this->size--;
        if (this->size <= this->capacity / 4) {
            this->capacity = this->capacity / 2;
            this->resize(this->capacity);
        }
    }
    void Display() {
        for (size_t i = 0; i < this->size; i++) {
            std::cout << this->vec[i] << " ";
        }
        std::cout << std::endl;
    }
    size_t len() {
        return this->size;
    }
    size_t cap() {
        return this->capacity;
    }
    ~Vector() {
        delete[] vec;
    }
};

int main() {
    Vector<int> vec;
    for(size_t i = 0; i < 30; i++) {
        vec.push_back(i);
    }
    for (size_t i = 0; i < 26; i++) {
        vec.pop_back();
    }
    std::cout << vec.cap() << std::endl;
    vec.Display();
}

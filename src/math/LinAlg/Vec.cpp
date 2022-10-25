//
// Created by alex on 24.10.2022.
//

#include "Vec.h"

namespace LA {
    template<typename T, int N>
    Vec<T, N>::Vec() {
        for (int i = 0; i < N; i++) arr[i] = 0;
    }

    template<typename T, int N>
    Vec<T, N>::Vec(std::initializer_list<T> vals) {
        int i = 0;
        for (const auto& v : vals ){
            arr[i++] = v;
        }
    }

    template<typename T, int N>
    Vec<T, N>::Vec(const std::array<T, N> &array) {
        for (int i = 0; i < N; i++) arr[i] = array[i];
    }

    template<typename T, int N>
    inline Vec<T, N> Vec<T, N>::add(const Vec<T, N> &vec) const {
        std::array<T, N> buffer;
        for (int i = 0; i < N; i++) buffer[i] = vec[i] + this->arr[i];
        return Vec<T, N>(buffer);
    }

    template<typename T, int N>
    inline void Vec<T, N>::add_to(const Vec<T, N> &vec) {
        for (int i = 0; i < N; i++) this->arr[i] += vec[i];
    }

    template<typename T, int N>
    inline Vec<T, N> Vec<T, N>::mul(T val) const {
        std::array<T, N> buffer;
        for (int i = 0; i < N; i++) buffer[i] = val * this->arr[i];
        return Vec<T, N>(buffer);
    }

    template<typename T, int N>
    inline void Vec<T, N>::mul_to(T val) {
        for (int i = 0; i < N; i++) this->arr[i] *= val;
    }

    template<typename T, int N>
    Vec<T, N> Vec<T, N>::operator+(const Vec<T, N> &vec) const {
        return add(vec);
    }

    template<typename T, int N>
    void Vec<T, N>::operator+=(const Vec<T, N> &vec) {
        add_to(vec);
    }

    template<typename T, int N>
    Vec<T, N> Vec<T, N>::operator*(T val) const {
        return mul(val);
    }

    template<typename T, int N>
    void Vec<T, N>::operator*=(T val) {
        mul_to(val);
    }

    template<typename T, int N>
    T Vec<T, N>::operator[](int index) const {
        return this->arr[index];
    }

    template<typename T, int N>
    inline void Vec<T, N>::set(const Vec<T, N> &vec) {
        for (int i = 0; i < N; i++) this->arr[i] = vec[i];
    }

    template<typename T, int N>
    Vec<T, N>& Vec<T, N>::operator=(const Vec<T, N> &vec) {
        set(vec);
        return this;
    }

    template<typename T, int N>
    std::ostream& operator<<(std::ostream &os, const Vec<T, N> &vec) {
        os << vec.arr;
        return os;
    }

    template<typename T, int N>
    int Vec<T, N>::operator==(const Vec<T, N> &vec) {
        for(int i = 0; i < N; i++) {
            if (arr[i] != vec[i]) return false;
        }
        return true;
    }

    template<typename T, int N>
    const std::array<T, N> &Vec<T, N>::getArray() const {
        return this->arr;
    }
}

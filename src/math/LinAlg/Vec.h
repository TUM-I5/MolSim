//
// Created by alex on 24.10.2022.
//
#pragma once

#include <array>
#include <initializer_list>

namespace LA {
    template<typename T, int N>
    class Vec {
    private:
        std::array<T, N> arr;
    public:
        Vec();

        Vec(std::initializer_list<T> vals);

        Vec(const std::array<T, N> &array);

        inline Vec<T, N> add(const Vec<T, N> &vec) const;

        inline void add_to(const Vec<T, N> &vec);

        inline Vec<T, N> mul(T val) const;

        inline void mul_to(T val);

        inline void set(const Vec<T, N> &vec);

        Vec<T, N> operator+(const Vec<T, N> &vec) const;

        void operator+=(const Vec<T, N> &vec);

        Vec<T, N> operator*(T val) const;

        void operator*=(T val);

        T operator[](int index) const;

        Vec& operator=(const Vec<T, N> &vec);

        int operator==(const Vec<T, N> &vec);

        friend std::ostream& operator<<(std::ostream& os, const Vec<T,N>& vec);

        const std::array<T, N>& getArray() const;
    };
}



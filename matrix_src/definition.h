#pragma once

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <vector>

namespace maykitbo {

template <class T>
class Matrix {
    public:
        using value_t = T;
        using i_type = unsigned int;
        using base = std::vector<T>;
        class Algebra;

        Matrix() noexcept;
        Matrix(i_type rows, i_type cols, T value);
        Matrix(i_type rows, i_type cols);
        Matrix(i_type rows);
        template<class ForwardIt>
        Matrix(i_type rows, i_type cols, ForwardIt begin, ForwardIt end);
        Matrix(i_type rows, i_type cols, base &&data);
        Matrix(i_type rows, i_type cols, std::istream &is);
        Matrix(std::istream &is);
        Matrix(const std::initializer_list<std::initializer_list<T>> &data);
        Matrix(i_type rows, i_type cols, const std::function<T(void)> &func);
        Matrix(i_type rows, i_type cols, const std::function<T(i_type, i_type)> &func);
        Matrix(const Matrix &other);
        Matrix(Matrix &&other);

        T &operator()(i_type row, i_type col) noexcept;
        const T &operator()(i_type row, i_type col) const noexcept;
        i_type GetRows() const noexcept;
        i_type GetCols() const noexcept;

        Matrix &operator=(const Matrix &other);
        Matrix &operator=(Matrix &&other);
        void Fill(T value);
        template<class ForwardIt>
        void Fill(ForwardIt begin, ForwardIt end);
        void Fill(base &&data);
        void Fill(const std::initializer_list<std::initializer_list<T>> &data);
        void Fill(const std::function<T(void)> &func);
        void Fill(const std::function<T(i_type, i_type)> &func);

        void ForEach(const std::function<void(T &)> &f);
        void ForEach(const std::function<void(i_type, i_type, T &)> &f);
        void ForEach(const std::function<void(const T &)> &f) const;
        void ForEach(const std::function<void(i_type, i_type, const T &)> &f) const;

        void Resize(i_type rows, i_type cols, T value = T());

        void InsertRow(i_type row, T value = T());
        template<class ForwardIt>
        void InsertRow(i_type row, ForwardIt begin, ForwardIt end);
        void InsertRow(i_type row, const std::function<T(void)> &func);
        void InsertRow(i_type row, const std::function<T(i_type, i_type)> &func);

        void InsertCol(i_type col, T value = T());
        template<class ForwardIt>
        void InsertCol(i_type col, ForwardIt begin, ForwardIt end);
        void InsertCol(i_type col, const std::function<T(void)> &func);
        void InsertCol(i_type col, const std::function<T(i_type, i_type)> &func);

        void RemoveRow(i_type row);
        void RemoveCol(i_type col);

        Matrix &operator+=(const Matrix &other);
        Matrix operator+(const Matrix &other) const;
        Matrix &operator+=(T value);
        Matrix operator+(T value) const;
        Matrix &operator-=(const Matrix &other);
        Matrix operator-(const Matrix &other) const;
        Matrix &operator-=(T value);
        Matrix operator-(T value) const;
        Matrix &operator*=(const Matrix &other);
        Matrix operator*(const Matrix &other) const;
        Matrix &operator*=(const T &value);
        Matrix operator*(const T &value) const;
        Matrix &operator/=(const T &value);
        Matrix operator/(const T &value) const;
        Matrix operator-() const;

        bool operator==(const Matrix &other) const noexcept;
        bool operator!=(const Matrix &other) const noexcept;
        void SetComparePrecision(double precision) noexcept;

        T *Data() noexcept;
        const T *Data() const noexcept;
        base &DataVector() noexcept;
        const base &DataVector() const noexcept;

        void Print(std::ostream &os = std::cout) const;
        void PrintFull(std::ostream &os = std::cout) const;
        void Read(std::istream &is = std::cin);
        void ReadFull(std::istream &is = std::cin);

        friend std::ostream &operator<<(std::ostream &os, const Matrix &matrix)
        {
            matrix.Print(os);
            return os;
        }
        friend std::istream &operator>>(std::istream &is, Matrix &matrix)
        {
            matrix.Read(is);
            return is;
        }

    private:

        i_type rows_;
        i_type cols_;
        base data_;
        double fp_compare_precision_{1e-6};
};

}  // namespace maykitbo

#pragma once

#include "definition.h"

namespace maykitbo {

template <class T>
void Matrix<T>::Algebra::Sum(const Matrix &a, const T value, Matrix &c)
{
    if (a.rows_ != c.rows_ || a.cols_ != c.cols_)
        throw std::runtime_error("Algebra::Sum: different sizes");

    const T *a_data = a.Data();
    T *c_data = c.Data();
    unsigned size = a.rows_ * a.cols_;
    for (i_type k = 0; k < size; ++k)
    {
        c_data[k] = a_data[k] + value;
    }
}

template <class T>
void Matrix<T>::Algebra::Mul(const Matrix &a, const T value, Matrix &c)
{
    if (a.rows_ != c.rows_ || a.cols_ != c.cols_)
        throw std::runtime_error("Algebra::Mul: different sizes");

    const T *a_data = a.Data();
    T *c_data = c.Data();
    unsigned size = a.rows_ * a.cols_;
    for (i_type k = 0; k < size; ++k)
    {
        c_data[k] = a_data[k] * value;
    }
}

template <class T>
void Matrix<T>::Algebra::Sum(const Matrix &a, const Matrix &b, Matrix &c)
{
    if (a.rows_ != b.rows_ || a.cols_ != b.cols_ || a.rows_ != c.rows_ || a.cols_ != c.cols_)
        throw std::runtime_error("Algebra::Sum: different sizes");

    const T *a_data = a.Data();
    const T *b_data = b.Data();
    T *c_data = c.Data();
    unsigned size = a.rows_ * a.cols_;
    // for (i_type i = 0; i < a.rows_; ++i)
    // {
    //     for (i_type j = 0; j < a.cols_; ++j)
    //     {
    //         c_data[i * a.cols_ + j] =
    //             a_data[i * a.cols_ + j] + b_data[i * a.cols_ + j];
    //     }
    // }
    for (i_type k = 0; k < size; ++k)
    {
        c_data[k] = a_data[k] + b_data[k];
    }
}


template <class T>
void Matrix<T>::Algebra::Sub(const Matrix &a, const Matrix &b, Matrix &c)
{
    if (a.rows_ != b.rows_ || a.cols_ != b.cols_ || a.rows_ != c.rows_ || a.cols_ != c.cols_)
        throw std::runtime_error("Algebra::Sub: different sizes");

    const T *a_data = a.Data();
    const T *b_data = b.Data();
    T *c_data = c.Data();
    unsigned size = a.rows_ * a.cols_;
    // for (i_type i = 0; i < a.rows_; ++i) {
    //     for (i_type j = 0; j < a.cols_; ++j)
    //     {
    //         c_data[i * a.cols_ + j] =
    //             a_data[i * a.cols_ + j] - b_data[i * a.cols_ + j];
    //     }
    // }
    for (i_type k = 0; k < size; ++k)
    {
        c_data[k] = a_data[k] - b_data[k];
    }
}

template <class T>
void Matrix<T>::Algebra::Mul(const Matrix &a, const Matrix &b, Matrix &c)
{
    if (a.cols_ != b.rows_ || a.rows_ != c.rows_ || b.cols_ != c.cols_)
        throw std::runtime_error("Algebra::Mul: different sizes");

    const T *a_data = a.Data();
    const T *b_data = b.Data();
    T *c_data = c.Data();
    for (i_type i = 0; i < a.rows_; ++i)
    {
        for (i_type j = 0; j < b.cols_; ++j)
        {
            T sum = 0;
            for (i_type k = 0; k < a.cols_; ++k)
            {
                sum += a_data[i * a.cols_ + k] * b_data[k * b.cols_ + j];
            }
            c_data[i * b.cols_ + j] = sum;
        }
    }
}

template <class T>
void Matrix<T>::Algebra::MulABT(const Matrix &a, const Matrix &b, Matrix &c)
{
    if (a.cols_ != b.cols_ || a.rows_ != c.rows_ || b.rows_ != c.cols_)
        throw std::runtime_error("Algebra::MulABT: different sizes");

    const T *a_data = a.Data();
    const T *b_data = b.Data();
    T *c_data = c.Data();
    for (i_type i = 0; i < a.rows_; ++i)
    {
        for (i_type j = 0; j < b.rows_; ++j)
        {
            T sum = 0;
            for (i_type k = 0; k < a.cols_; ++k)
            {
                sum += a_data[i * a.cols_ + k] * b_data[j * b.cols_ + k];
            }
            c_data[i * b.rows_ + j] = sum;
        }
    }
}

template <class T>
void Matrix<T>::Algebra::MulATB(const Matrix &a, const Matrix &b, Matrix &c)
{
    if (a.rows_ != b.rows_ || a.cols_ != c.rows_ || b.cols_ != c.cols_)
        throw std::runtime_error("Algebra::MulATB: different sizes");

    const T *a_data = a.Data();
    const T *b_data = b.Data();
    T *c_data = c.Data();
    for (i_type i = 0; i < a.cols_; ++i)
    {
        for (i_type j = 0; j < b.cols_; ++j)
        {
            T sum = 0;
            for (i_type k = 0; k < a.rows_; ++k)
            {
                sum += a_data[k * a.cols_ + i] * b_data[k * b.cols_ + j];
            }
            c_data[i * b.cols_ + j] = sum;
        }
    }
}

// template <class T>
// void Matrix<T>::Algebra::Divide(const Matrix &a, Matrix &b, T value)
// {
//     if (a.rows_ != b.rows_ || a.cols_ != b.cols_) {
//     throw std::runtime_error("Algebra::Divide: different sizes");
//     }
//     const T *a_data = a.Data();
//     T *b_data = b.Data();
//     for (i_type i = 0; i < a.rows_; ++i) {
//     for (i_type j = 0; j < a.cols_; ++j) {
//     b_data[i * a.cols_ + j] = a_data[i * a.cols_ + j] / value;
//     }
//     }
// }

template <class T>
void Matrix<T>::Algebra::Inverse(const Matrix &a, Matrix &c)
{
    if (a.rows_ != a.cols_ || a.rows_ != c.rows_ || a.cols_ != c.cols_)
        throw std::runtime_error("Algebra::Inverse: incorrect sizes");

    T det = Determinant(a);
    if (det == 0)
    {
        std::cerr << "Algebra::Inverse: matrix is singular (non-invertible)\n";
        return;
    }

    int n = a.rows_;

    Matrix<T> minors(n, n);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            Matrix<T> minor = Minor(a, i, j);
            T minor_det = Determinant(minor);
            minors(i, j) = (i + j) % 2 == 0 ? minor_det : -minor_det;
        }
    }
    Matrix<T> adj = Transpose(minors);
    // Divide(adj, c, det);
    Mul(adj, 1 / det, c);
}

template <class T>
T Matrix<T>::Algebra::Determinant(const Matrix &a)
{
    if (a.rows_ != a.cols_)
        throw std::runtime_error("Algebra::Determinant: matrix is not square");

    int n = a.rows_;
    if (n == 1)
    {
        return a(0, 0);
    }
    if (n == 2)
    {
        return a(0, 0) * a(1, 1) - a(0, 1) * a(1, 0);
    }
    T det = 0;
    for (int j = 0; j < n; j++)
    {
        T cofactor = a(0, j) * Determinant(Minor(a, 0, j));
        if (j % 2 == 0)
            det += cofactor;
        else
            det -= cofactor;
    }
    return det;
}

template <class T>
Matrix<T> Matrix<T>::Algebra::Minor(const Matrix &a, int row, int col)
{
    if (row < 0 || row >= a.rows_ || col < 0 || col >= a.cols_)
        throw std::runtime_error("Algebra::Minor: incorrect sizes");

    Matrix minor(a.rows_ - 1, a.cols_ - 1);
    int minor_row = 0;
    for (int i = 0; i < a.rows_; i++) {
        if (i == row)
            continue;

        int minor_col = 0;
        for (int j = 0; j < a.cols_; j++) {
            if (j == col)
                continue;

            minor(minor_row, minor_col) = a(i, j);
            ++minor_col;
        }
        ++minor_row;
    }
    return minor;
}

template <class T>
Matrix<T> Matrix<T>::Algebra::Transpose(const Matrix &a)
{
    Matrix<T> transpose(a.cols_, a.rows_);
    for (int i = 0; i < a.rows_; i++)
    {
        for (int j = 0; j < a.cols_; j++)
        {
            transpose(j, i) = a(i, j);
        }
    }
    return transpose;
}
















} // namespace maykitbo

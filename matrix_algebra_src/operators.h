#pragma once

#include "definition.h"

namespace maykitbo {

template <class T>
Matrix<T> &Matrix<T>::operator+=(const Matrix &other)
{
    Algebra::Sum(*this, other, *this);
    return *this;
}

template <class T>
Matrix<T> Matrix<T>::operator+(const Matrix &other) const
{
    Matrix result(rows_, cols_);
    Algebra::Sum(*this, other, result);
    return result;
}

template <class T>
Matrix<T> &Matrix<T>::operator+=(T value)
{
    Algebra::Sum(*this, value, *this);
    return *this;
}

template <class T>
Matrix<T> Matrix<T>::operator+(T value) const
{
    Matrix result(rows_, cols_);
    Algebra::Sum(*this, value, result);
    return result;
}

template <class T>
Matrix<T> &Matrix<T>::operator-=(const Matrix &other)
{
    Algebra::Sum(*this, other * -1, *this);
    return *this;
}

template <class T>
Matrix<T> Matrix<T>::operator-(const Matrix &other) const
{
    Matrix result(rows_, cols_);
    Algebra::Sum(*this, other * -1, result);
    return result;
}

template <class T>
Matrix<T> &Matrix<T>::operator-=(T value)
{
    Algebra::Sum(*this, value * -1, *this);
    return *this;
}

template <class T>
Matrix<T> Matrix<T>::operator-(T value) const
{
    Matrix result(rows_, cols_);
    Algebra::Sum(*this, value * -1, result);
    return result;
}

template <class T>
Matrix<T> &Matrix<T>::operator*=(const Matrix &other)
{
    Algebra::Mul(*this, other, *this);
    return *this;
}

template <class T>
Matrix<T> Matrix<T>::operator*(const Matrix &other) const
{
    Matrix result(rows_, other.cols_);
    Algebra::Mul(*this, other, result);
    return result;
}

template <class T>
Matrix<T> &Matrix<T>::operator*=(const T &value)
{
    Algebra::Mul(*this, value, *this);
    return *this;
}

template <class T>
Matrix<T> Matrix<T>::operator*(const T &value) const
{
    Matrix result(rows_, cols_);
    Algebra::Mul(*this, value, result);
    return result;
}

template <class T>
Matrix<T> &Matrix<T>::operator/=(const T &value)
{
    Algebra::Mul(*this, 1 / value, *this);
    return *this;
}

template <class T>
Matrix<T> Matrix<T>::operator/(const T &value) const
{
    Matrix result(rows_, cols_);
    Algebra::Mul(*this, 1 / value, result);
    return result;
}

template <class T>
Matrix<T> Matrix<T>::operator-() const
{
    Matrix result(rows_, cols_);
    Algebra::Mul(*this, -1, result);
    return result;
}


} // namespace maykitbo

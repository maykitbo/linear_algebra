#pragma once

#include "../matrix.h"

namespace maykitbo {

template<class T>
class Matrix<T>::Algebra
{
    public:
        static void Sum(const Matrix<T> &a, const T value, Matrix<T> &c);
        static void Mul(const Matrix &a, const T value, Matrix &c);

        static void Sum(const Matrix<T> &a, const Matrix<T> &b, Matrix<T> &c);
        static void Sub(const Matrix &a, const Matrix &b, Matrix &c);
        static void MulABT(const Matrix &a, const Matrix &b, Matrix &c);
        static void MulATB(const Matrix &a, const Matrix &b, Matrix &c);
        // static void MulATBT(const Matrix &a, const Matrix &b, Matrix &c);
        static void Inverse(const Matrix &a, Matrix &c);
        static T Determinant(const Matrix &a);
        static Matrix Minor(const Matrix &a, int row, int col);
        static Matrix Transpose(const Matrix &a);

        static void Mul(const Matrix &a, const Matrix &b, Matrix &c);

};




} // namespace maykitbo

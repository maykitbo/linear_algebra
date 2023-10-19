#pragma once

#include "definition.h"

namespace maykitbo {

template <class T>
T &Matrix<T>::operator()(i_type row, i_type col) noexcept {
    return data_[row * cols_ + col];
}

template <class T>
const T &Matrix<T>::operator()(i_type row, i_type col) const noexcept {
    return data_[row * cols_ + col];
}

template <class T>
typename Matrix<T>::i_type Matrix<T>::GetRows() const noexcept {
    return rows_;
}

template <class T>
typename Matrix<T>::i_type Matrix<T>::GetCols() const noexcept {
    return cols_;
}

template <class T>
bool Matrix<T>::operator==(const Matrix &other) const noexcept
{
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        return false;
    }
    if constexpr (std::is_floating_point_v<T>)
    {
        for (i_type i = 0; i < rows_; ++i)
        {
            for (i_type j = 0; j < cols_; ++j)
            {
                if (std::abs(data_[cols_ * i + j] - other.data_[cols_ * i + j]) > fp_compare_precision_)
                {
                    return false;
                }
            }
        }
    }
    else
    {
        for (i_type i = 0; i < rows_; ++i)
        {
            for (i_type j = 0; j < cols_; ++j)
            {
                if (data_[cols_ * i + j] != other.data_[cols_ * i + j])
                {
                    return false;
                }
            }
        }
    }
    return true;
}

template <class T>
bool Matrix<T>::operator!=(const Matrix &other) const noexcept
{
    return !(*this == other);
}

template <class T>
void Matrix<T>::Print(std::ostream &os) const
{
    for (i_type i = 0; i < rows_; ++i)
    {
        for (i_type j = 0; j < cols_; ++j)
        {
            os << data_[i * cols_ + j] << '\t';
        }
        os << '\n';
    }
}

template <class T>
void Matrix<T>::PrintFull(std::ostream &os) const
{
    os << rows_ << '\t' << cols_ << '\n';
    Print(os);
}

template <class T>
void Matrix<T>::Read(std::istream &is)
{
    for (auto &value : data_)
    {
        is >> value;
    }
}

template <class T>
void Matrix<T>::ReadFull(std::istream &is)
{
    is >> rows_ >> cols_;
    data_.resize(rows_ * cols_);
    Read(is);
}

template <class T>
T *Matrix<T>::Data() noexcept
{
    return data_.data();
}

template <class T>
const T *Matrix<T>::Data() const noexcept
{
    return data_.data();
}

template <class T>
typename Matrix<T>::base &Matrix<T>::DataVector() noexcept
{
    return data_;
}

template <class T>
const typename Matrix<T>::base &Matrix<T>::DataVector() const noexcept
{
    return data_;
}

template <class T>
void Matrix<T>::SetComparePrecision(double precision) noexcept
{
    fp_compare_precision_ = precision;
}

} // namespace maykitbo

#pragma once

#include "definition.h"

namespace maykitbo {

template <class T>
Matrix<T>::Matrix() noexcept
    : rows_(0)
    , cols_(0)
    {}

template <class T>
Matrix<T>::Matrix(i_type rows, i_type cols, T value)
    : rows_(rows)
    , cols_(cols)
    , data_(rows * cols, value)
    {}

template <class T>
Matrix<T>::Matrix(i_type rows, i_type cols)
    : rows_(rows)
    , cols_(cols)
    , data_(rows * cols)
    {}

template <class T>
Matrix<T>::Matrix(i_type rows)
    : Matrix(rows, rows)
    {}

template <class T>
template<class ForwardIt>
Matrix<T>::Matrix(i_type rows, i_type cols, ForwardIt begin, ForwardIt end)
    : Matrix(rows, cols)
{
    if (rows * cols != std::distance(begin, end))
        throw std::invalid_argument("Matrix::iterator_constructor: iterator range size does not match matrix size");
    std::copy(begin, end, data_.begin());
}

template <class T>
Matrix<T>::Matrix(i_type rows, i_type cols, base &&data)
    : rows_(rows)
    , cols_(cols)
    , data_(std::move(data))
{
    if (rows * cols != data_.size())
        throw std::invalid_argument("Matrix::move_constructor: data vector size does not match matrix size");
}

template <class T>
Matrix<T>::Matrix(i_type rows, i_type cols, std::istream &is)
    : Matrix(rows, cols)
{
    for (auto &value : data_)
    {
        is >> value;
    }
}

template <class T>
Matrix<T>::Matrix(std::istream &is)
{
    is >> rows_ >> cols_;
    data_.resize(rows_ * cols_);
    for (auto &value : data_) {
        is >> value;
    }
}

template <class T>
Matrix<T>::Matrix(const std::initializer_list<std::initializer_list<T>> &data)
    : Matrix(data.size(), data.begin()->size())
{
    i_type i = 0;
    for (const auto &row : data)
    {
        for (const auto &value : row)
        {
            data_[i++] = value;
        }
    }
}

template <class T>
Matrix<T>::Matrix(i_type rows, i_type cols, const std::function<T(void)> &func)
    : Matrix(rows, cols)
{
    std::generate(data_.begin(), data_.end(), func);
}

template <class T>
Matrix<T>::Matrix(i_type rows, i_type cols, const std::function<T(i_type, i_type)> &func)
    : rows_(rows)
    , cols_(cols)
    , data_(rows * cols)
{
    for (i_type i = 0; i < rows_; ++i)
    {
        for (i_type j = 0; j < cols_; ++j)
        {
            data_[i * cols_ + j] = func(i, j);
        }
    }
}

template <class T>
Matrix<T>::Matrix(const Matrix &other)
    : rows_(other.rows_)
    , cols_(other.cols_)
    , data_(other.data_)
    {}

template <class T>
Matrix<T>::Matrix(Matrix &&other)
    : rows_(other.rows_)
    , cols_(other.cols_)
    , data_(std::move(other.data_))
{
    other.rows_ = 0;
    other.cols_ = 0;
}

} // namespace maykitbo

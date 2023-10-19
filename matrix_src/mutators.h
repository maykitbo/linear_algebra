#pragma once

#include "definition.h"

namespace maykitbo {

template <class T>
Matrix<T> &Matrix<T>::operator=(const Matrix &other)
{
    if (this != &other)
    {
        rows_ = other.rows_;
        cols_ = other.cols_;
        data_ = other.data_;
    }
    return *this;
}

template <class T>
Matrix<T> &Matrix<T>::operator=(Matrix &&other)
{
    if (this != &other)
    {
        rows_ = other.rows_;
        cols_ = other.cols_;
        data_ = std::move(other.data_);
        other.rows_ = 0;
        other.cols_ = 0;
    }
    return *this;
}


template <class T>
void Matrix<T>::Fill(T value)
{
    std::fill(data_.begin(), data_.end(), value);
}

template <class T>
template<class ForwardIt>
void Matrix<T>::Fill(ForwardIt begin, ForwardIt end)
{
    if (std::distance(begin, end) != rows_ * cols_)
        throw std::invalid_argument("Matrix::Fill: iterator range size does not match matrix size");
    std::copy(begin, end, data_.begin());
}

template <class T>
void Matrix<T>::Fill(base &&data)
{
    data_ = std::move(data);
    if (rows_ * cols_ != data_.size())
        throw std::invalid_argument("Matrix::Fill: data vector size does not match matrix size");
}

template <class T>
void Matrix<T>::Fill(const std::initializer_list<std::initializer_list<T>> &data)
{
    rows_ = data.size();
    cols_ = data.begin()->size();
    data_.resize(rows_ * cols_);
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
void Matrix<T>::Fill(const std::function<T(void)> &func)
{
    std::generate(data_.begin(), data_.end(), func);
}

template <class T>
void Matrix<T>::Fill(const std::function<T(i_type, i_type)> &func)
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
void Matrix<T>::ForEach(const std::function<void(T &)> &f)
{
    for (auto &value : data_)
    {
        f(value);
    }
}

template <class T>
void Matrix<T>::ForEach(const std::function<void(i_type, i_type, T &)> &f)
{
    for (i_type i = 0; i < rows_; ++i)
    {
        for (i_type j = 0; j < cols_; ++j)
        {
            f(i, j, data_[i * cols_ + j]);
        }
    }
}

template <class T>
void Matrix<T>::ForEach(const std::function<void(const T &)> &f) const
{
    for (const auto &value : data_)
    {
        f(value);
    }
}

template <class T>
void Matrix<T>::ForEach(const std::function<void(i_type, i_type, const T &)> &f) const
{
    for (i_type i = 0; i < rows_; ++i)
    {
        for (i_type j = 0; j < cols_; ++j)
        {
            f(i, j, data_[i * cols_ + j]);
        }
    }
}

template <class T>
void Matrix<T>::Resize(i_type rows, i_type cols, T value)
{
    if (cols == 0 || rows == 0)
    {
        cols_ = 0;
        rows_ = 0;
        data_.clear();
        return;
    }

    if (cols > cols_)
    {
        for (i_type i = rows_; i >= 1; --i)
        {
            data_.insert(data_.begin() + i * cols_, cols - cols_, value);
        }
    }
    else if (cols < cols_)
    {
        for (i_type i = rows_; i >= 1; --i)
        {
            data_.erase(data_.begin() + i * cols_ - cols_ + cols, data_.begin() + i * cols_);
        }
    }
    if (rows > rows_)
    {
        data_.insert(data_.end(), (rows - rows_) * cols, value);
    }
    else if (rows < rows_)
    {
        data_.erase(data_.end() - (rows_ - rows) * cols, data_.end());
    }
    cols_ = cols;
    rows_ = rows;
}

template <class T>
void Matrix<T>::InsertRow(i_type row, T value)
{
    if (row > rows_)
        throw std::out_of_range("Matrix::InsertRow: row index out of range");
    
    if (cols_ == 0)
        cols_ = 1;

    data_.insert(data_.begin() + row * cols_, cols_, value);
    ++rows_;
}

template <class T>
template<class ForwardIt>
void Matrix<T>::InsertRow(i_type row, ForwardIt begin, ForwardIt end)
{
    if (row > rows_)
        throw std::out_of_range("Matrix::InsertRow: row index out of range");
    if (std::distance(begin, end) != cols_ && cols_ != 0)
        throw std::invalid_argument("Matrix::InsertRow: invalid range");
    
    if (cols_ == 0)
        cols_ = std::distance(begin, end);
    
    data_.insert(data_.begin() + row * cols_, begin, end);
    ++rows_;
}

template <class T>
void Matrix<T>::InsertRow(i_type row, const std::function<T(void)> &func)
{
    if (row > rows_)
        throw std::out_of_range("Matrix::InsertRow: row index out of range");

    if (cols_ == 0)
        cols_ = 1;

    data_.insert(data_.begin() + row * cols_, cols_);
    std::generate(data_.begin() + row * cols_, data_.begin() + (row + 1) * cols_, func);
    ++rows_;
}

template <class T>
void Matrix<T>::InsertRow(i_type row, const std::function<T(i_type, i_type)> &func)
{
    if (row > rows_)
        throw std::out_of_range("Matrix::InsertRow: row index out of range");

    if (cols_ == 0)
        cols_ = 1;

    data_.insert(data_.begin() + row * cols_, cols_);
    for (i_type j = 0; j < cols_; ++j)
    {
        data_[row * cols_ + j] = func(row, j);
    }
    ++rows_;
}


template <class T>
void Matrix<T>::InsertCol(i_type col, T value)
{
    if (col > cols_)
        throw std::out_of_range("Matrix::InsertCol: col index out of range");
    
    if (rows_ == 0)
        rows_ = 1;

    for (int i = (int)rows_ - 1; i >= 0; --i)
    {
        data_.insert(data_.begin() + i * cols_ + col, value);
    }
    ++cols_;
}

template <class T>
template<class ForwardIt>
void Matrix<T>::InsertCol(i_type col, ForwardIt begin, ForwardIt end)
{
    if (col > cols_)
        throw std::out_of_range("Matrix::InsertCol: col index out of range");
    if (std::distance(begin, end) != rows_ && rows_ != 0)
        throw std::invalid_argument("Matrix::InsertCol: invalid range");

    if (rows_ == 0)
        rows_ = std::distance(begin, end);

    for (int i = (int)rows_ - 1; i >= 0; --i)
    {
        data_.insert(data_.begin() + i * cols_ + col, *(begin + i));
    }
    ++cols_;
}

template <class T>
void Matrix<T>::InsertCol(i_type col, const std::function<T(void)> &func)
{
    if (col > cols_)
        throw std::out_of_range("Matrix::InsertCol: col index out of range");
    
    if (rows_ == 0)
        rows_ = 1;

    for (int i = (int)rows_ - 1; i >= 0; --i)
    {
        data_.insert(data_.begin() + i * cols_ + col, func());
    }
    ++cols_;
}

template <class T>
void Matrix<T>::InsertCol(i_type col, const std::function<T(i_type, i_type)> &func)
{
    if (col > cols_)
        throw std::out_of_range("Matrix::InsertCol: col index out of range");
    
    if (rows_ == 0)
        rows_ = 1;

    for (int i = (int)rows_ - 1; i >= 0; --i)
    {
        data_.insert(data_.begin() + i * cols_ + col, func(i, col));
    }
    ++cols_;
}

template <class T>
void Matrix<T>::RemoveRow(i_type row)
{
    if (row >= rows_)
        throw std::out_of_range("Matrix::RemoveRow: row index out of range");
    
    data_.erase(data_.begin() + row * cols_, data_.begin() + (row + 1) * cols_);
    --rows_;
    if (rows_ == 0)
    {
        cols_ = 0;
        data_.clear();
    }
}

template <class T>
void Matrix<T>::RemoveCol(i_type col)
{
    if (col >= cols_)
        throw std::out_of_range("Matrix::RemoveCol: col index out of range");
    
    for (int i = (int)rows_ - 1; i >= 0; --i)
    {
        data_.erase(data_.begin() + i * cols_ + col);
    }
    --cols_;
    if (cols_ == 0)
    {
        rows_ = 0;
        data_.clear();
    }
}



















} // namespace maykitbo

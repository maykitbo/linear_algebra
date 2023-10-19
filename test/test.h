#pragma once

#include "../matrix.h"

#include <gtest/gtest.h>
#include "utility/utility.h"

using namespace maykitbo;

class MatrixTest : public ::testing::Test
{
    public:
        MatrixTest() = default;

    protected:
        template<class T>
        void SizeTest(const Matrix<T> &m, unsigned r, unsigned c);

        template<class T>
        void ValueTest(const Matrix<T> &m, const std::function<void(unsigned, unsigned, const T&)> &func);
};

template<class T>
void MatrixTest::SizeTest(const Matrix<T> &m, unsigned r, unsigned c)
{
    EXPECT_EQ(m.GetRows(), r);
    EXPECT_EQ(m.GetCols(), c);
    EXPECT_EQ(m.DataVector().size(), r * c);
}

template<class T>
void MatrixTest::ValueTest(const Matrix<T> &m, const std::function<void(unsigned, unsigned, const T&)> &func)
{
    for (unsigned i = 0; i < m.GetRows(); ++i)
    {
        for (unsigned j = 0; j < m.GetCols(); ++j)
        {
            func(i, j, m(i, j));
        }
    }
}

#define TESTFUNC1(m, v) \
    ValueTest<decltype(m)::value_t>(m, \
        [&](unsigned i [[maybe_unused]], unsigned j [[maybe_unused]], const decltype(m)::value_t &d [[maybe_unused]]) \
        { EXPECT_EQ(d, v); });

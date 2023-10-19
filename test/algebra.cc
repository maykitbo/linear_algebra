#include "../matrix_algebra.h"

#include <gtest/gtest.h>

using namespace maykitbo;

TEST(AlgebraTest, matrix_num_sum_static)
{
    Matrix<int> a
    {
        {1, 2, 3},
        {4, 5, 6}
    };
    Matrix<int> b
    {
        {5, 6, 7},
        {8, 9, 10}
    };
    Matrix<int> c(2, 3);
    Matrix<int>::Algebra::Sum(a, 4, c);
    EXPECT_EQ(b, c);
    Matrix<int> d(3, 3);
    EXPECT_ANY_THROW(Matrix<int>::Algebra::Sum(a, 4, d));
}

TEST(AlgebraTest, matrix_num_sum_operator_1)
{
    Matrix<int> a
    {
        {1, 2, 3},
        {4, 5, 6}
    };
    Matrix<int> b
    {
        {5, 6, 7},
        {8, 9, 10}
    };
    Matrix<int> c = a + 4;
    EXPECT_EQ(b, c);
}

TEST(AlgebraTest, matrix_num_sum_operator_2)
{
    Matrix<int> a
    {
        {1, 2, 3},
        {4, 5, 6}
    };
    Matrix<int> b
    {
        {5, 6, 7},
        {8, 9, 10}
    };
    a += 4;
    EXPECT_EQ(b, a);
}

TEST(AlgebraTest, matrix_num_sum_operator_3)
{
    Matrix<double> a
    {
        {1.0, 2.0},
        {3.0, 4.0}
    };
    Matrix<double> b
    {
        {-1.0, 0.0},
        {1.0, 2.0}
    };
    Matrix<double> c = a - 2.0;
    EXPECT_EQ(c, b);
}

TEST(AlgebraTest, matrix_num_sum_operator_4)
{
    Matrix<double> a
    {
        {1.0, 2.0},
        {3.0, 4.0}
    };
    Matrix<double> b
    {
        {-1.0, 0.0},
        {1.0, 2.0}
    };
    a -= 2.0;
    EXPECT_EQ(b, a);
}

TEST(AlgebraTest, matrix_num_mul_static)
{
    Matrix<double> a
    {
        {1.0, 2.0},
        {4.0, 5.0},
        {7.0, 8.0}
    };
    Matrix<double> b
    {
        {0.1, 0.2},
        {0.4, 0.5},
        {0.7, 0.8}
    };
    Matrix<double> c(3, 2);
    Matrix<double>::Algebra::Mul(a, 0.1, c);
    EXPECT_EQ(b, c);
    Matrix<double> d(2, 2);
    EXPECT_ANY_THROW(Matrix<double>::Algebra::Mul(a, 0.1, d));
}

TEST(AlgebraTest, matrix_num_mul_operator_1)
{
    Matrix<double> a
    {
        {1.0, 2.0},
        {4.0, 5.0},
        {7.0, 8.0}
    };
    Matrix<double> b
    {
        {0.1, 0.2},
        {0.4, 0.5},
        {0.7, 0.8}
    };
    Matrix<double> c = a * 0.1;
    EXPECT_EQ(b, c);
}

TEST(AlgebraTest, matrix_num_mul_operator_2)
{
    Matrix<double> a
    {
        {9.0, 9.0},
        {-1.1, -1.1}
    };
    Matrix<double> b
    {
        {-1.8, -1.8},
        {0.22, 0.22}
    };
    a *= -0.2;
    EXPECT_EQ(b, a);
}

TEST(AlgebraTest, matrix_num_mul_operator_3)
{
    Matrix<double> a
    { 
        {33.0, 44.0, 55.0, 66.0},
        {77.0, 88.0, 99.0, 0.0}
    };
    Matrix<double> b
    {
        {3.0, 4.0, 5.0, 6.0},
        {7.0, 8.0, 9.0, 0.0}
    };
    Matrix<double> c = a / 11.0;
    EXPECT_EQ(b, c);
}

TEST(AlgebraTest, matrix_num_mul_operator_4)
{
    Matrix<double> a
    { 
        {33.0, 44.0, 55.0, 66.0},
        {77.0, 88.0, 99.0, 0.0}
    };
    Matrix<double> b
    {
        {3.0, 4.0, 5.0, 6.0},
        {7.0, 8.0, 9.0, 0.0}
    };
    a /= 11.0;
    EXPECT_EQ(b, a);
}

TEST(AlgebraTest, matrix_matrix_sum_static)
{
    Matrix<int> a
    {
        {1, 2, 3},
        {4, 5, 6}
    };
    Matrix<int> b
    {
        {5, 6, 7},
        {8, 9, 10}
    };
    Matrix<int> d
    {
        {6, 8, 10},
        {12, 14, 16}
    };
    Matrix<int> c(2, 3);
    Matrix<int>::Algebra::Sum(a, b, c);
    EXPECT_EQ(d, c);
    Matrix<int>::Algebra::Sum(b, a, c);
    EXPECT_EQ(d, c);
    Matrix<int> e(3, 3);
    EXPECT_ANY_THROW(Matrix<int>::Algebra::Sum(a, b, e));
    EXPECT_ANY_THROW(Matrix<int>::Algebra::Sum(a, e, b));
    EXPECT_ANY_THROW(Matrix<int>::Algebra::Sum(e, c, e));
}

TEST(AlgebraTest, matrix_matrix_sum_operator_1)
{
    Matrix<int> a;
    Matrix<int> b;
    auto c = a + b;
    EXPECT_EQ(a, c);
    Matrix<int> d;
    EXPECT_EQ(d, c);
}

TEST(AlgebraTest, matrix_matrix_sum_operator_2)
{
    Matrix<int> a{
        {1, 2},
        {-5, -5}
    };
    Matrix<int> b{
        {3, 4},
        {5, 6}
    };
    Matrix<int> c{
        {4, 6},
        {0, 1}
    };
    a += b;
    EXPECT_EQ(c, a);
}

TEST(AlgebraTest, matrix_matrix_sub_static)
{
    Matrix<int> a
    {
        {1, 2, 3},
        {4, 5, 6}
    };
    Matrix<int> b
    {
        {5, 6, 7},
        {8, 9, 10}
    };
    Matrix<int> d
    {
        {4, 4, 4},
        {4, 4, 4}
    };
    Matrix<int> c(2, 3);
    Matrix<int>::Algebra::Sub(b, a, c);
    EXPECT_EQ(d, c);
    Matrix<int>::Algebra::Sub(a, b, c);
    EXPECT_EQ(-d, c);
    Matrix<int> e(3, 3);
    EXPECT_ANY_THROW(Matrix<int>::Algebra::Sub(a, b, e));
    EXPECT_ANY_THROW(Matrix<int>::Algebra::Sub(a, e, b));
    EXPECT_ANY_THROW(Matrix<int>::Algebra::Sub(e, c, e));
}

TEST(AlgebraTest, matrix_matrix_sub_operator)
{
    Matrix<int> a
    {
        {1, 2},
        {3, 4}
    };
    Matrix<int> b
    {
        {5, 6},
        {7, 8}
    };
    auto c = a - b;
    Matrix<int> d(a);
    d -= b;
    EXPECT_EQ(c, d);
}



#include "test.h"

using namespace maykitbo;

TEST_F(MatrixTest, operator_1_2)
{
    Matrix<double> m1(2, 3, 5.5);
    Matrix<double> m2 = m1;
    EXPECT_EQ(m1, m2);
    Matrix<double> m3 = std::move(m1);
    EXPECT_EQ(m2, m3);
    SizeTest(m1, 0, 0);
}

TEST_F(MatrixTest, fill_1)
{
    Matrix<double> m(2, 3);
    m.Fill(5.5);
    SizeTest(m, 2, 3);
    TESTFUNC1(m, 5.5);
}

TEST_F(MatrixTest, fill_2)
{
    Matrix<double> m(2, 3);
    std::vector<double> v{1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    m.Fill(v.begin(), v.end());
    SizeTest(m, 2, 3);
    TESTFUNC1(m, i * 3 + j + 1.0);
    EXPECT_EQ(m.DataVector(), v);
    v.resize(3);
    EXPECT_ANY_THROW(m.Fill(v.begin(), v.end()));
}

TEST_F(MatrixTest, fill_3)
{
    Matrix<double> m(2, 3);
    std::vector<double> v{1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    m.Fill(std::move(v));
    SizeTest(m, 2, 3);
    TESTFUNC1(m, i * 3 + j + 1.0);
    EXPECT_EQ(v.size(), 0);
}

TEST_F(MatrixTest, fill_4)
{
    Matrix<double> m(2, 3);
    m.Fill({{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}});
    SizeTest(m, 2, 3);
    TESTFUNC1(m, i * 3 + j + 1.0);
}

TEST_F(MatrixTest, fill_5)
{
    Matrix<double> m(2, 3);
    m.Fill([] { return 5.5; });
    SizeTest(m, 2, 3);
    TESTFUNC1(m, 5.5);
}

TEST_F(MatrixTest, fill_6)
{
    Matrix<double> m(2, 3);
    m.Fill([] (unsigned i, unsigned j) { return i * 3 + j + 1.0; });
    SizeTest(m, 2, 3);
    TESTFUNC1(m, i * 3 + j + 1.0);
}

TEST_F(MatrixTest, for_each_1)
{
    Matrix<int> m(5, 5, 5);
    m.ForEach([] (int &d) {
        EXPECT_EQ(d, 5);
    });
}

TEST_F(MatrixTest, for_each_2)
{
    Matrix<int> m
    {
        {5, 6, 7},
        {8, 9, 10}
    };
    m.ForEach([] (unsigned i, unsigned j, int &d) {
        EXPECT_EQ(d, i * 3 + j + 5);
    });
}

TEST_F(MatrixTest, resize_1)
{
    Matrix<int> m1
    {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    Matrix<int> m2
    {
        {1, 2, 3, 9, 9},
        {4, 5, 6, 9, 9},
        {7, 8, 9, 9, 9},
        {9, 9, 9, 9, 9},
        {9, 9, 9, 9, 9}
    };
    m1.Resize(5, 5, 9);
    EXPECT_EQ(m1, m2);
}

TEST_F(MatrixTest, resize_2)
{
    Matrix<int> m1
    {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    Matrix<int> m2
    {
        {1, 2, 3, 0},
        {4, 5, 6, 0},
        {7, 8, 9, 0}
    };
    m1.Resize(3, 4);
    EXPECT_EQ(m1, m2);
}

TEST_F(MatrixTest, resize_3)
{
    Matrix<int> m1
    {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    Matrix<int> m2
    {
        {1, 2, 3, 0, 0, 0},
        {4, 5, 6, 0, 0, 0}
    };
    m1.Resize(2, 6);
    EXPECT_EQ(m1, m2);
}

TEST_F(MatrixTest, resize_4)
{
    Matrix<int> m1
    {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    Matrix<int> m2
    {
        {1, 2},
        {4, 5},
        {7, 8},
        {11, 11},
        {11, 11},
        {11, 11}
    };
    m1.Resize(6, 2, 11);
    EXPECT_EQ(m1, m2);
}

TEST_F(MatrixTest, resize_5)
{
    Matrix<int> m1
    {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 0, 1, 2},
        {3, 4, 5, 6}
    };
    Matrix<int> m2
    {
        {1, 2},
    };
    m1.Resize(1, 2);
    EXPECT_EQ(m1, m2);
}

TEST_F(MatrixTest, resize_6)
{
    Matrix<int> m1
    {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 0, 1, 2},
        {3, 4, 5, 6}
    };
    Matrix<int> m2
    {
        {1},
    };
    m1.Resize(1, 1);
    EXPECT_EQ(m1, m2);
}

TEST_F(MatrixTest, resize_7)
{
    Matrix<int> m1
    {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    Matrix<int> m2;
    m1.Resize(0, 0, 11);
    EXPECT_EQ(m1, m2);
}

TEST_F(MatrixTest, resize_8)
{
    Matrix<int> m1;
    Matrix<int> m2(4, 7, 77);
    m1.Resize(4, 7, 77);
    EXPECT_EQ(m1, m2);
}

TEST_F(MatrixTest, insert_row_1)
{
    Matrix<int> m1
    {
        {1, 2, 3},
        {4, 5, 6}
    };
    Matrix<int> m2
    {
        {1, 2, 3},
        {7, 7, 7},
        {4, 5, 6}
    };
    m1.InsertRow(1, 7);
    EXPECT_EQ(m1, m2);
    EXPECT_ANY_THROW(m1.InsertRow(4, 7));
}

TEST_F(MatrixTest, insert_row_2)
{
    std::vector<int> v{1, 4, 7};
    Matrix<int> m1
    {
        {1, 2, 3},
        {4, 5, 6}
    };
    Matrix<int> m2
    {
        {1, 4, 7},
        {1, 2, 3},
        {4, 5, 6}
    };
    m1.InsertRow(0, v.begin(), v.end());
    EXPECT_EQ(m1, m2);
    EXPECT_ANY_THROW(m1.InsertRow(4, v.begin(), v.end()));
    v.resize(2);
    EXPECT_ANY_THROW(m1.InsertRow(0, v.begin(), v.end()));
}

TEST_F(MatrixTest, insert_row_3)
{
    Matrix<int> m1
    {
        {1, 2, 3, 10},
        {4, 5, 6, 10},
        {7, 8, 9, 10}
    };
    Matrix<int> m2
    {
        {1, 2, 3, 10},
        {4, 5, 6, 10},
        {7, 8, 9, 10},
        {1, 2, 3, 4}
    };
    int i = 0;
    m1.InsertRow(3, [&i] {
        return ++i;
    });
    EXPECT_EQ(m1, m2);
}

TEST_F(MatrixTest, insert_row_4)
{
    Matrix<int> m1
    {
        {1, 2, 3, 10},
        {4, 5, 6, 10},
        {7, 8, 9, 10}
    };
    Matrix<int> m2
    {
        {1, 2, 3, 10},
        {4, 5, 6, 10},
        {7, 8, 9, 10},
        {0, 3, 6, 9}
    };
    m1.InsertRow(3, [] (unsigned i, unsigned j) {
        return i * j;
    });
    EXPECT_EQ(m1, m2);
}

TEST_F(MatrixTest, insert_row_5)
{
    Matrix<int> m1;
    Matrix<int> m2
    {
        {1, 2, 3},
    };
    std::vector<int> v{1, 2, 3};
    m1.InsertRow(0, v.begin(), v.end());
    EXPECT_EQ(m1, m2);
}

TEST_F(MatrixTest, insert_row_6)
{
    Matrix<int> m1;
    Matrix<int> m2(1, 1, 1);
    m1.InsertRow(0, 1);
    EXPECT_EQ(m1, m2);
}

TEST_F(MatrixTest, insert_col_1)
{
    Matrix<int> m1
    {
        {1, 2, 3},
        {4, 5, 6}
    };
    Matrix<int> m2
    {
        {0, 1, 2, 3},
        {0, 4, 5, 6}
    };
    m1.InsertCol(0);
    EXPECT_EQ(m1, m2);
    EXPECT_ANY_THROW(m1.InsertCol(5));
}

TEST_F(MatrixTest, insert_col_2)
{
    std::vector<int> v{1, 4};
    Matrix<int> m1
    {
        {1, 2, 3},
        {4, 5, 6}
    };
    Matrix<int> m2
    {
        {1, 2, 3, 1},
        {4, 5, 6, 4}
    };
    m1.InsertCol(3, v.begin(), v.end());
    EXPECT_EQ(m1, m2);
    EXPECT_ANY_THROW(m1.InsertCol(10, v.begin(), v.end()));
    v.resize(4);
    EXPECT_ANY_THROW(m1.InsertCol(0, v.begin(), v.end()));
}

TEST_F(MatrixTest, insert_col_3)
{
    Matrix<int> m1
    {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    Matrix<int> m2
    {
        {1, 2, 12, 3},
        {4, 5, 11, 6},
        {7, 8, 10, 9},
    };
    int i = 9;
    m1.InsertCol(2, [&i] {
        return ++i;
    });
    EXPECT_EQ(m1, m2);
}

TEST_F(MatrixTest, insert_col_4)
{
    Matrix<int> m1
    {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    Matrix<int> m2
    {
        {1, 2, 3, 0},
        {4, 5, 6, 3},
        {7, 8, 9, 6},
    };
    m1.InsertCol(3, [] (unsigned i, unsigned j) {
        return i * j;
    });
    EXPECT_EQ(m1, m2);
}

TEST_F(MatrixTest, insert_col_5)
{
    Matrix<int> m1;
    Matrix<int> m2(1, 1, 7);
    m1.InsertCol(0, [] {
        return 7;
    });
    EXPECT_EQ(m1, m2);
}

TEST_F(MatrixTest, insert_col_6)
{
    Matrix<int> m1;
    Matrix<int> m2{
        {7},
        {7},
        {7},
        {8}
    };
    std::vector<int> v{7, 7, 7, 8};
    m1.InsertCol(0, v.begin(), v.end());
    EXPECT_EQ(m1, m2);
}

TEST_F(MatrixTest, remove_row_1)
{
    Matrix<int> m1
    {
        {1, 2, 3, 4},
        {5, 6, 7, 8}
    };
    Matrix<int> m2
    {
        {1, 2, 3, 4}
    };
    m1.RemoveRow(1);
    EXPECT_EQ(m1, m2);
    EXPECT_ANY_THROW(m1.RemoveRow(1));
}

TEST_F(MatrixTest, remove_row_2)
{
    Matrix<int> m1
    {
        {1, 2, 3, 4},
    };
    Matrix<int> m2;
    m1.RemoveRow(0);
    EXPECT_EQ(m1, m2);
    EXPECT_ANY_THROW(m1.RemoveRow(1));
}

TEST_F(MatrixTest, remove_col_1)
{
    Matrix<int> m1
    {
        {1, 2, 3, 4},
        {5, 6, 7, 8}
    };
    Matrix<int> m2
    {
        {1, 2, 3},
        {5, 6, 7}
    };
    m1.RemoveCol(3);
    EXPECT_EQ(m1, m2);
    EXPECT_ANY_THROW(m1.RemoveCol(3));
}

TEST_F(MatrixTest, remove_col_2)
{
    Matrix<int> m1
    {
        {1, 2},
        {5, 6}
    };
    Matrix<int> m2;
    m1.RemoveCol(1);
    m1.RemoveCol(0);
    EXPECT_EQ(m1, m2);
    EXPECT_ANY_THROW(m1.RemoveCol(1));
}



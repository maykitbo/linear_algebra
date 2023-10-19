#include "test.h"

using namespace maykitbo;

TEST_F(MatrixTest, constructor_1)
{
    Matrix<int> m;
    SizeTest(m, 0, 0);
}

TEST_F(MatrixTest, constructor_2)
{
    Matrix<std::string> m(2, 3, "hello matrix");
    SizeTest(m, 2, 3);
    TESTFUNC1(m, "hello matrix");
}

TEST_F(MatrixTest, constructor_3)
{
    Matrix<int> m(5, 6);
    SizeTest(m, 5, 6);
}

TEST_F(MatrixTest, constructor_4)
{
    Matrix<int> m(8);
    SizeTest(m, 8, 8);
}

TEST_F(MatrixTest, constructor_5)
{
    std::vector<double> v{1.0, 2.0, 3.0, 4.0};
    Matrix<double> m1(1, 4, v.begin(), v.end());
    SizeTest(m1, 1, 4);
    TESTFUNC1(m1, j + 1.0);
    EXPECT_EQ(m1.DataVector(), v);

    Matrix<double> m2(4, 1, v.begin(), v.end());
    SizeTest(m2, 4, 1);
    TESTFUNC1(m2, i + 1.0);
    EXPECT_EQ(m2.DataVector(), v);

    EXPECT_ANY_THROW(Matrix<double>(3, 2, v.begin(), v.end()));
}

TEST_F(MatrixTest, constructor_6)
{
    std::vector<double> v1{1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    Matrix<double> m1(3, 2, std::move(v1));
    SizeTest(m1, 3, 2);
    TESTFUNC1(m1, i * 2 + j + 1.0);
    EXPECT_EQ(v1.size(), 0);

    std::vector<double> v2{1.0, 2.0, 3.0, 4.0, 5.0, 6.0};

    Matrix<double> m2(2, 3, std::move(v2));
    SizeTest(m2, 2, 3);
    TESTFUNC1(m2, i * 3 + j + 1.0);
    EXPECT_EQ(v2.size(), 0);

    EXPECT_ANY_THROW(Matrix<double>(3, 2, std::move(v1)));
    EXPECT_ANY_THROW(Matrix<double>(3, 2, std::move(v2)));
    EXPECT_ANY_THROW(Matrix<double>(3, 9, std::move(v2)));
}

TEST_F(MatrixTest, constructor_7)
{
    Matrix<int> m(2, 3, [](unsigned i , unsigned j) { return i + j; });
    SizeTest(m, 2, 3);
    TESTFUNC1(m, i + j);
}

TEST_F(MatrixTest, constructor_8)
{
    std::istringstream iss("9 9 9 9 9 9 21");
    Matrix<int> m(2, 3, iss);
    SizeTest(m, 2, 3);
    TESTFUNC1(m, 9);

    // Matrix<int> mcin(2, 3, std::cin);

    std::ofstream ofs("test8.txt");
    ofs << "0 -1 -2 -3 -4 -5";
    ofs.close();
    std::ifstream ifs("test8.txt");
    Matrix<int> mifs(2, 3, ifs);
    SizeTest(mifs, 2, 3);
    TESTFUNC1(mifs, -1 * (int)(i * 3 + j));
    ifs.close();
    std::remove("test8.txt");
}

TEST_F(MatrixTest, constructor_9)
{
    std::istringstream iss(
        "4 4 0 0 0 0 0 1 2 3 0 2 4 6 0 3 6 9");
    Matrix<int> m(iss);
    SizeTest(m, 4, 4);
    TESTFUNC1(m, i * j);

    // Matrix<int> mcin(std::cin);

    std::ofstream ofs("test9.txt");
    ofs << "1 1 99";
    ofs.close();
    std::ifstream ifs("test9.txt");
    Matrix<int> mifs(ifs);
    SizeTest(mifs, 1, 1);
    TESTFUNC1(mifs, 99);
    ifs.close();
    std::remove("test9.txt");
}

TEST_F(MatrixTest, constructor_10)
{
    Matrix<int> m{
        {5, 6, 7},
        {8, 9, 10}
    };
    SizeTest(m, 2, 3);
    TESTFUNC1(m, i * 3 + j + 5);
}

TEST_F(MatrixTest, constructor_11)
{
    std::vector<long> v(6);
    int k = 0;
    Matrix<long> m(2, 3, [&k, &v] {
        v[k] = Random::Int<long>(-10000, 10000);
        return v[k++];
    });
    SizeTest(m, 2, 3);
    TESTFUNC1(m, v[i * 3 + j]);
}

TEST_F(MatrixTest, constructor_12)
{
    // std::vector<long> v(6);
    Matrix<long> m1(10, 3, [] { return Random::Int<long>(-10000, 10000); });
    Matrix<long> m2(10, 3, [&m1] (unsigned i, unsigned j) {
        return m1(j, i);
    });
    SizeTest(m1, 10, 3);
    SizeTest(m2, 10, 3);
    TESTFUNC1(m2, m1(j, i));
}

TEST_F(MatrixTest, constructor_13_14)
{
    Matrix<double> m1(2, 3, 5.5);
    Matrix<double> m2(m1);
    EXPECT_EQ(m1, m2);
    Matrix<double> m3(std::move(m1));
    EXPECT_EQ(m2, m3);
    SizeTest(m1, 0, 0);
}

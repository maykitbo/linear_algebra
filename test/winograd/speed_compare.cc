// #include "tests.h"
#include "strassen_winograd/winograd_parallel.h"
#include "strassen_winograd/winograd.h"
#include "strassen_winograd/strassen_parallel.h"
#include "strassen_winograd/strassen.h"
#include "../utility/utility.h"

#include "parser.h"

#include <set>

#include <fstream>
#include <map>

#include <cblas.h>

using namespace maykitbo;

template<class T>
struct blas_func;


template <>
struct blas_func<float> {
    constexpr static auto mul = cblas_sgemm;
};

template <>
struct blas_func<double> {
    constexpr static auto mul = cblas_dgemm;
};

template<class T>
void MulBlas(const Matrix<T> &A, const Matrix<T> &B, Matrix<T> &C)
{
    blas_func<T>::mul(CblasRowMajor, CblasNoTrans, CblasNoTrans, A.GetRows(), B.GetCols(),
              A.GetCols(), 1.0, A.Data(), A.GetCols(), B.Data(), B.GetCols(), 0.0,
              C.Data(), B.GetCols());
}

template<class T>
void MulParallel(const Matrix<T> &a, const Matrix<T> &b, Matrix<T> &c)
{
    std::vector<std::thread> thrs;
    const unsigned threads = 8;
    const unsigned n = a.GetRows();
    const T *A = a.Data();
    const T *B = b.Data();
    T *C = c.Data();

    for (unsigned thc = 0; thc < threads; ++thc) {
        thrs.emplace_back([&, thc](){
            unsigned delta = n / threads;
            unsigned end = (thc == threads - 1 ? n : (thc + 1) * delta);
            for (unsigned i = thc * delta; i < end; ++i) {
                for (unsigned j = 0; j < n; ++j) {
                    T sum = 0;
                    for (unsigned l = 0; l < n; ++l) {
                        sum += A[i * n + l] * B[l * n + j];
                    }
                    C[i * n + j] = sum;
                }
            }
        });
    }
    for (auto &i : thrs) {
        i.join();
    }
}

template<class T>
void Mul2(const Matrix<T> &A, const Matrix<T> &B, Matrix<T> &C)
{
    T *c = C.Data();
    const T *a = A.Data();
    const T *b = B.Data();
    const unsigned n = A.GetRows();
    for (unsigned i = 0; i < n; ++i)
    {
        for (unsigned k = 0; k < n; ++k)
        {
            double r = a[i * n + k];
            for (unsigned j = 0; j < n; ++j)
            {
                c[i * n + j] += r * b[k * n + j];
            }
        }
    }
}

template<class T>
void Mul2Parallel(const Matrix<T> &a, const Matrix<T> &b, Matrix<T> &c)
{
    std::vector<std::thread> thrs;
    const unsigned threads = 8;
    const unsigned n = a.GetRows();
    const T *A = a.Data();
    const T *B = b.Data();
    T *C = c.Data();

    for (unsigned thc = 0; thc < threads; ++thc) {
        thrs.emplace_back([&, thc](){
            unsigned delta = n / threads;
            unsigned end = (thc == threads - 1 ? n : (thc + 1) * delta);
            for (unsigned i = thc * delta; i < end; ++i) {
                for (unsigned k = 0; k < n; ++k)
                {
                    double r = A[i * n + k];
                    for (unsigned j = 0; j < n; ++j)
                    {
                        C[i * n + j] += r * B[k * n + j];
                    }
                }
            }
        });
    }
    for (auto &i : thrs) {
        i.join();
    }
}

template<class T, class Unit>
std::vector<int64_t> All(const unsigned N, unsigned int repeat) {
    Matrix<T> A(N, N, Random::Easy<T>::R(static_cast<T>(-1000), static_cast<T>(1000)));
    Matrix<T> B(N, N, Random::Easy<T>::R(static_cast<T>(-1000), static_cast<T>(1000)));
    Matrix<T> C(N, N);
    return Time::Compare<Unit>(repeat, [&] {
            Winograd<T>::Mul(A, B, C);
        }, [&] {
            WinogradP<T>::Mul(A, B, C);
            // v2::Winograd<T>::Mul(A, B, C);
        }, [&] {
            Strassen<T>::Mul(A, B, C);
        }, [&] {
            StrassenP<T>::Mul(A, B, C);
        }, [&] {
            MulBlas<T>(A, B, C);
        }, [&] {
            Mul2Parallel(A, B, C);
        }
    );
}

template<class T>
void Print(const unsigned N, int repeat)
{
    auto result = All<T, Time::ns>(N, repeat);
    std::cout << N << ":\n";
    for (auto &i : result) {
        std::cout << '\t' << Time::GetAdapt<Time::ns>(i) << '\n';
    }
    std::cout << '\n';
}

void TestConf()
{
    // Print<double>(8, 100000);
    // Print<double>(25, 10000);
    // Print<double>(64, 10000);
    // Print<double>(78, 1000);
    // Print<double>(128, 1000);
    // Print<double>(133, 1000);
    // Print<double>(211, 1000);
    // Print<double>(255, 400);
    // Print<double>(256, 300);
    // Print<double>(512, 100);
    Print<double>(1024, 6);
    Print<double>(2048, 3);
    Print<double>(4096, 1);
}


void ToFileConf()
{
    std::vector<unsigned> v1;
    std::vector<unsigned> v2;
    for (int k = 1; k < 16; ++k)
    {
        v1.push_back(k);
    }
    unsigned p2 = 2;
    for (int k = 0; k < 10; ++k)
    {
        v2.push_back(p2);
        p2 *= 2;
    }
    int g = 0;
    std::set<unsigned> s;
    for (unsigned k = 8; k < 128; )
    {
        s.insert(k);
        if (++g % 2 == 0) {
            k += 3;
        } else {
            k += 4;
        }
    }
    for (unsigned k = 128; k < 256; )
    {
        s.insert(k);
        if (++g % 2) {
            k += 5;
        } else {
            k += 6;
        }
    }
    for (unsigned k = 256; k < 512; )
    {
        s.insert(k);
        if (++g % 2) {
            k += 11;
        } else {
            k += 8;
        }
    }
    for (unsigned k = 512; k < 1024; )
    {
        s.insert(k);
        if (++g % 2) {
            k += 31;
        } else {
            k += 16;
        }
    }
    for (unsigned k = 1024; k < 2048; )
    {
        s.insert(k);
        if (++g % 2) {
            k += 73;
        } else {
            k += 32;
        }
    }
    for (unsigned k = 2048; k < 4096; )
    {
        s.insert(k);
        if (++g % 2) {
            k += 107;
        } else {
            k += 64;
        }
    }
    s.insert(4096);
    for (auto &i : v1)
    {
        for (auto &j : v2)
        {
            auto r = i * j;
            if (r <= 4096 && r >= 8 && s.find(r) == s.end())
            {
                s.insert(r);
            }
        }
    }
    std::cout << s.size() << '\n';


    input_data result(7);
    for (auto &i : s)
    {
        unsigned repeat = 50000;
        if (i > 64) repeat = 5000;
        if (i > 128) repeat = 600;
        if (i > 256) repeat = 145;
        if (i > 512) repeat = 19;
        if (i > 1024) repeat = 6;
        if (i > 2048) repeat = 3;

        auto r = All<double, Time::ns>(i, repeat);
        result[0].push_back(i);
        for (unsigned k = 0; k < r.size(); ++k)
            result[k + 1].push_back(r[k]);
        

        std::cout << i << ": " << r[0] << ' ' << r[1] << ' ' << r[2] << ' ' << r[3] << ' ' << r[4] << ' ' << r[5] << '\n';
    }
    Parser::ToFile("test8", result);

    // for (int k = 8; k < 128; ++k)
    // {
    //     ToFile<double>(file, k, 100);
    // }
    // std::cout << "\n128\n";
    // for (int k = 128; k < 256; k += 4)
    // {
    //     ToFile<double>(file, k, 30);
    //     if (k % 8 == 0)
    //     {
    //         ToFile<double>(file, k + 1, 30);
    //         std::cout << k << ' ';
    //     }
    // }
    // std::cout << "\n256\n";
    // for (int k = 256; k < 512; k += 8)
    // {
    //     ToFile<double>(file, k, 10);
    //     if (k % 16 == 0)
    //     {
    //         ToFile<double>(file, k + 1, 10);
    //         std::cout << k << ' ';
    //     }
    // }
    // std::cout << "\n512\n";
    // for (int k = 512; k < 1024; k += 16)
    // {
    //     ToFile<double>(file, k, 5);
    //     if (k % 32 == 0)
    //     {
    //         ToFile<double>(file, k + 1, 5);
    //         std::cout << k << ' ';
    //     }
    // }
    // std::cout << "\n1024\n";
    // for (int k = 1024; k < 2048; k += 32)
    // {
    //     ToFile<double>(file, k, 3);
    //     if (k % 64 == 0)
    //     {
    //         ToFile<double>(file, k + 1, 3);
    //         std::cout << k << ' ';
    //     }
    // }
    // std::cout << '\n';

}


int main() {

    // TestConf();
    ToFileConf();

    return 0;
}

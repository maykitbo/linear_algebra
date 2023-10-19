#include "strassen_winograd/winograd.h"
#include "strassen_winograd/winograd_parallel.h"
#include "strassen_winograd/strassen.h"
#include "strassen_winograd/strassen_parallel.h"
#include "../utility/utility.h"
#include "../../matrix_algebra.h"
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

using type = double;

void OneTest(const unsigned N)
{
    Matrix<type> A(N, N, [] { return Random::Easy<type>::R(-100.0, 100.0); });
    Matrix<type> B(N, N, [] { return Random::Easy<type>::R(-100.0, 100.0); });
    Matrix<type> C(N, N);

    // Winograd<type>::Mul(A, B, C);
    // WinogradP<type>::Mul(A, B, C);

    // MulBlas<type>(A, B, C);
    // Mul2<type>(A, B, C);
    Mul2Parallel<type>(A, B, C);
}

int main()
{
    // OneTest(128);
    // OneTest(2048);

    const unsigned k = 3245;

    // unsigned long long t1 = 0, t2 = 0;

    // for (unsigned k = 256; k < 512; ++k)
    // {
        Matrix<type> A(k, k, [] { return Random::Easy<type>::R(-100.0, 100.0); });
        Matrix<type> B(k, k, [] { return Random::Easy<type>::R(-100.0, 100.0); });
        Matrix<type> C1(k, k);
    //     Matrix<type> C2(k, k);

    Winograd<type>::Mul(A, B, C1);
    // auto result = Time::Compare(2, [&] {
    //     StrassenP<type>::Mul(A, B, C1);
    // }, [&] {
    //     WinogradP<type>::Mul(A, B, C1);
    // });

    // std::cout << k << ": " << Time::GetAdapt(result[0]) << "\t" << Time::GetAdapt(result[1]) << '\n';


    //     auto result = Time::Compare<Time::ns>(5, [&] {
    //         v2::Winograd<type>::Mul(A, B, C1);
    //     }, [&] {
    //         Winograd<type>::Mul(A, B, C2);
    //     });
    //     t1 += result[0];
    //     t2 += result[1];
    //     std::cout << k << ": " << Time::GetAdapt(result[0]) << "\t" << Time::GetAdapt(result[1]) << '\n';
    // }

    // std::cout << "Total: " << Time::GetAdapt(t1 / 256) << "\t" << Time::GetAdapt(t2 / 256) << '\n';

    

    return 0;
}

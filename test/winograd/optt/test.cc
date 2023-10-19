#include "../strassen_winograd/winograd.h"
#include "../strassen_winograd/winograd2.0.h"
#include "../strassen_winograd/winograd_parallel.h"
#include "../strassen_winograd/winograd3.0.h"
#include "../../utility/utility.h"
#include "../../../matrix_algebra.h"

#include <cblas.h>

using namespace maykitbo;


template<class T>struct blas_func;
template <>struct blas_func<float>
    { constexpr static auto mul = cblas_sgemm; };
template <>struct blas_func<double>
    { constexpr static auto mul = cblas_dgemm; };

template<class T>
void MulBlas(const Matrix<T> &A, const Matrix<T> &B, Matrix<T> &C)
{
    blas_func<T>::mul(CblasRowMajor, CblasNoTrans, CblasNoTrans, A.GetRows(), B.GetCols(),
              A.GetCols(), 1.0, A.Data(), A.GetCols(), B.Data(), B.GetCols(), 0.0,
              C.Data(), B.GetCols());
}

void Mul2(const Matrix<double> &A, const Matrix<double> &B, Matrix<double> &C)
{
    const double *a = A.Data();
    const double *b = B.Data();
    double *c = C.Data();
    const unsigned n = A.GetCols();
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

int main()
{
    const unsigned N = 8192;
    Matrix<double> A(N, N, [] { return Random::Normal<double>(1.0, 10.0); });
    Matrix<double> B(N, N, [] { return Random::Normal<double>(1.0, 10.0); });
    Matrix<double> C1(N, N);
    Matrix<double> C2(N, N);
    Matrix<double> C3(N, N);
    Matrix<double> C4(N, N);
    Matrix<double> C5(N, N);

    std::cout << N << '\n';

    // Basic::Winograd<double>::Mul(A, B, C1);
    // Basic20::Winograd<double>::Mul(A, B, C2);
    // Parallel::Winograd<double>::Mul(A, B, C3);
    // Parallel30::Winograd<double>::Mul(A, B, C4);
    // C5 = A * B;
    // C5.SetComparePrecision(1e-3);
    // std::cout << (C5 == C1) << ' ' << (C5 == C2) << ' ' << (C5 == C3) << ' ' << (C5 == C4) << '\n';
    // return 0;


    auto result = Time::Compare<Time::mcs>(1, [&] {
        // Basic::Winograd<double>::Mul(A, B, C1);
    }, [&] {
        // Basic20::Winograd<double>::Mul(A, B, C2);
    }, [&] {
        Parallel::Winograd<double>::Mul(A, B, C3);
    }, [&] {
        Parallel30::Winograd<double>::Mul(A, B, C4);
    }, [&] {
        MulBlas(A, B, C5);
    });

    std::cout << "Basic: " << result[0] << '\n'
              << "Basic20: " << result[1] << '\n'
              << "Parallel: " << result[2] << '\n'
              << "Parallel30: " << result[3] << '\n'
              << "Blas: " << result[4] << '\n';
    


    return 0;
}

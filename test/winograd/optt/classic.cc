#include "../../utility/utility.h"

#include <iostream>

typedef std::vector<double> matrix;

void Mul1(const matrix &a, const matrix&b, matrix &c, unsigned n)
{
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            double sum = 0;
            for (int k = 0; k < n; ++k)
            {
                sum += a[i * n + k] * b[k * n + j];
            }
            c[i * n + j] = sum;
        }
    }
}

void Mul2(const matrix &a, const matrix&b, matrix &c, unsigned n)
{
    for (int i = 0; i < n; ++i)
    {
        for (int k = 0; k < n; ++k)
        {
            double r = a[i * n + k];
            for (int j = 0; j < n; ++j)
            {
                c[i * n + j] += r * b[k * n + j];
            }
        }
    }
}


using namespace maykitbo;

int main()
{
    unsigned N = 1024;
    matrix A(N * N);
    matrix B(N * N);
    matrix C1(N * N);
    matrix C2(N * N);

    for (int i = 0; i < N * N; ++i)
    {
        A[i] = Random::Normal<double>(1.0, 10.0);
        B[i] = Random::Normal<double>(1.0, 10.0);
    }

    // Mul1(A, B, C1, N);
    // Mul2(A, B, C2, N);

    // if (C1 == C2)
    // {
    //     std::cout << "Same" << std::endl;
    // }
    // else
    // {
    //     std::cout << "Different" << std::endl;
    // }

    auto result = Time::Compare<Time::mcs>(1, [&] {
        Mul1(A, B, C1, N);
    }, [&] {
        Mul2(A, B, C2, N);
    });

    std::cout << result[0] << ' ' << result[1] << '\n';


    return 0;
}

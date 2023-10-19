#include <gtest/gtest.h>

#include "../utility/m_random.h"
#include "../../matrix_algebra.h"

#ifdef WINOGRAD
    #include "strassen_winograd/winograd.h"
    #define CLASS_NAME Winograd
#elif defined WINOGRADP
    #include "strassen_winograd/winograd_parallel.h"
    #define CLASS_NAME WinogradP
#elif defined STRASSEN
    #include "strassen_winograd/strassen.h"
    #define CLASS_NAME Strassen
#elif defined STRASSENP
    #include "strassen_winograd/strassen_parallel.h"
    #define CLASS_NAME StrassenP
#else
    #error "No algorithm defined"
#endif

#define CONCAT(a, b) a##b
#define FUNCTIONAL_CLASS(class_name) CONCAT(class_name, _functional)
#define ERROR_CLASS(class_name) CONCAT(class_name, _error)

using namespace maykitbo;

template<class T, class W>
void Helper(unsigned int N, T random_min, T random_max, const W &w) {
    Matrix<T> A(N, N, [&] { return Random::Easy<T>::R(random_min, random_max); });
    Matrix<T> B(N, N, [&] { return Random::Easy<T>::R(random_min, random_max); });
    Matrix<T> C1(N, N);
    auto C2 = A * B;
    w.Mul(A, B, C1);
    C1.SetComparePrecision(1e-5);
    if (C1 != C2)
    {
        std::cout << '\t' << N << '\n';
        EXPECT_FALSE(true);
    }
}


template<class T>
void Functional(unsigned int N,
        T random_min = static_cast<T>(-10),
        T random_max = static_cast<T>(10)) {
    
    Helper<T>(N, random_min, random_max, CLASS_NAME<T>(N));  
}


#define __ONE_SIZE_TEST(T, n) \
    TEST(FUNCTIONAL_CLASS(CLASS_NAME), __##n##x##n) { \
        Functional<T>(n); \
    }


#define __ERROR_TEST(N, M, K, L) \
    TEST(ERROR_CLASS(CLASS_NAME), __##N##x##M##_dot_##K##x##L) { \
        Matrix<float> A(N, M); \
        Matrix<float> B(K, L); \
        Matrix<float> C(N, L); \
        EXPECT_ANY_THROW(CLASS_NAME<float>::Mul(A, B, C)); \
    }

__ONE_SIZE_TEST(double, 2)
__ONE_SIZE_TEST(long double, 3)
__ONE_SIZE_TEST(int, 4)
__ONE_SIZE_TEST(short, 5)
__ONE_SIZE_TEST(unsigned short, 6)
__ONE_SIZE_TEST(long, 7)
__ONE_SIZE_TEST(long long, 8)
__ONE_SIZE_TEST(unsigned int, 9)
__ONE_SIZE_TEST(unsigned long, 10)
__ONE_SIZE_TEST(unsigned long long, 11)
__ONE_SIZE_TEST(char, 12)
__ONE_SIZE_TEST(unsigned char, 13)
__ONE_SIZE_TEST(float, 14)


TEST(FUNCTIONAL_CLASS(CLASS_NAME), __from_15x15_to_64x64) {
    for (unsigned int k = 15; k <= 64; ++k) {
        Functional<double>(k, Random::Normal(0.0, 15.5), Random::Normal(-0.5, 1579.9));
    }
}

TEST(FUNCTIONAL_CLASS(CLASS_NAME), __from_65x65_to_128x128) {
    for (unsigned int k = 65; k <= 128; ++k) {
        Functional<int>(k);
    }
}

TEST(FUNCTIONAL_CLASS(CLASS_NAME), __from_129x129_to_512x512) {
    for (unsigned int k = 129; k <= 512; k += 21) {
        Functional<int>(k);
    }
    Functional<int>(512);
}

__ERROR_TEST(5, 5, 6, 6)
__ERROR_TEST(5, 6, 6, 5)
__ERROR_TEST(5, 6, 6, 7)
__ERROR_TEST(5, 6, 7, 8)
__ERROR_TEST(1, 2, 3, 4)
__ERROR_TEST(1, 1, 1, 1)
__ERROR_TEST(0, 0, 0, 0)




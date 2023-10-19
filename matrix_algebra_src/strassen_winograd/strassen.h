#pragma once

#include "../../matrix.h"

#include <vector>
#include <thread>
#include <memory>

#ifdef LEVEL_LOAD_TEST__P
    #include <map>
    #include <chrono>
    #define TIME std::chrono::high_resolution_clock::now
    #define DURATION(x) std::chrono::duration_cast<std::chrono::nanoseconds>(TIME() - x).count()
#endif

#define EVIL 7

namespace maykitbo {

template<class T>
class Strassen
{
    using M = Matrix<T>;
    using i_type = unsigned;
    using data_t = typename M::base;

    public:
        Strassen(i_type n, i_type odd_cap = 25, i_type strassen_cap = 17);
        void Execute(const M &A, const M &B, M &C);
        static void Mul(const M &A, const M &B, M &C,
                        i_type odd_cap = 25, i_type strassen_cap = 17);
        
        virtual ~Strassen() = default;

#ifdef LEVEL_LOAD_TEST__P
    inline static int64_t load{0};
#else
    protected:
#endif
        Strassen() = default;
        void InitAnalysis(i_type n, i_type odd_cap, i_type strassen_cap);

        struct Level;
        struct LevelEven;
        struct LevelOdd;
        struct LevelClassic;
        struct Level22;
        struct PointerBase;
        
        std::vector<std::unique_ptr<Level>> L_;
        i_type n_;
};

template<class T>
void Strassen<T>::Mul(const M &A, const M &B, M &C,
            i_type odd_cap, i_type strassen_cap)
{

    i_type n = A.GetCols();
    if (n <= 1 || B.GetCols() != n || C.GetCols() != n ||
        A.GetRows() != n || B.GetRows() != n || C.GetRows() != n)
    {
        throw std::invalid_argument("Matrix size not match");
    }
    Strassen<T> W(n, odd_cap, strassen_cap);
    W.Execute(A, B, C);
}

template<class T>
struct Strassen<T>::Level
{
    virtual void SW(const T *A, const T *B, T *C) = 0;
    Level *next;
    virtual ~Level() = default;
};

template<class T>
struct Strassen<T>::PointerBase
{
    T *A11, *A12, *A22, *B11, *B21, *B22;
    T *S1, *S2, *S3, *S4, *T1, *T2, *T3, *T4;
    T *R1, *R2, *R3, *R4, *R5, *R6, *R7;
    i_type n;

    PointerBase(i_type n);
    virtual ~PointerBase();
};

template<class T>
struct Strassen<T>::LevelEven : public Level, public PointerBase
{
    using Level::next;
    using PB = PointerBase;
    using PB::n;
    using PB::A11, PB::A12, PB::A22, PB::B11, PB::B21, PB::B22,
        PB::S1, PB::S2, PB::S3, PB::S4, PB::T1, PB::T2, PB::T3, PB::T4,
        PB::R1, PB::R2, PB::R3, PB::R4, PB::R5, PB::R6, PB::R7;

    LevelEven(i_type n)
        : PointerBase(n)
    {}
    virtual ~LevelEven() = default;
    virtual void SW(const T *A, const T *B, T *C) override;

#ifdef LEVEL_LOAD_TEST__P
    inline static std::map<i_type, std::pair<int64_t, int64_t>> level_load;
#endif
};

template<class T>
struct Strassen<T>::LevelOdd : public Level, public PointerBase
{
    using Level::next;
    using PB = PointerBase;
    using PB::n;
    using PB::A11, PB::A12, PB::A22, PB::B11, PB::B21, PB::B22,
        PB::S1, PB::S2, PB::S3, PB::S4, PB::T1, PB::T2, PB::T3, PB::T4,
        PB::R1, PB::R2, PB::R3, PB::R4, PB::R5, PB::R6, PB::R7;

    LevelOdd(i_type n) : PointerBase(n) {}
    virtual ~LevelOdd() = default;
    virtual void SW(const T *A, const T *B, T *C) override;

#ifdef LEVEL_LOAD_TEST__P
    inline static std::map<i_type, std::pair<int64_t, int64_t>> level_load;
#endif
};

template<class T>
struct Strassen<T>::LevelClassic final : public Level
{
    i_type n;
    LevelClassic(i_type n) : n(n) {}
    void SW(const T *A, const T *B, T *C) override;

#ifdef LEVEL_LOAD_TEST__P
    inline static int64_t load{0};
#endif
};

template<class T>
struct Strassen<T>::Level22 final : public Level
{
    using Level::next;
    void SW(const T *A, const T *B, T *C) override;   

#ifdef LEVEL_LOAD_TEST__P
    inline static int64_t load{0};
#endif
};

template<class T>
void Strassen<T>::InitAnalysis(i_type n, i_type odd_cap, i_type strassen_cap)
{
    bool odd = (n % 2 != 0);
    n += odd;
    bool cap = false;

    while ((n /= 2) != 1) {
        if ((odd && n < odd_cap) || n * 2 < strassen_cap) {
            L_.push_back(std::make_unique<LevelClassic>(n * 2 - odd));
            cap = true;
            break;
        }
        if (odd)
            L_.push_back(std::make_unique<LevelOdd>(n));
        else
            L_.push_back(std::make_unique<LevelEven>(n));

        odd = (n % 2 != 0);
        n += odd;
    }

    if (!cap) {
        L_.push_back(std::make_unique<Level22>());
    }

    for (int i = L_.size() - 1; i > 0; --i) {
        L_[i - 1]->next = &(*L_[i]);
    }
}

template<class T>
Strassen<T>::Strassen(i_type n, i_type odd_cap, i_type strassen_cap)
    : n_(n)
{
    if (n <= 1) {
        throw std::invalid_argument("Matrix size must be greater than 1");
    }

#ifdef LEVEL_LOAD_TEST__P
    auto time_p = TIME();
#endif

    InitAnalysis(n, odd_cap, strassen_cap);
    
#ifdef LEVEL_LOAD_TEST__P
    load += DURATION(time_p);
#endif

}


template<class T>
void Strassen<T>::Execute(const M &A, const M &B, M &C)
{
    if (A.GetCols() != n_ || B.GetCols() != n_ || C.GetCols() != n_ ||
        A.GetRows() != n_ || B.GetRows() != n_ || C.GetRows() != n_)
    {
        throw std::invalid_argument("Matrix size not match ");
    }
    L_[0]->SW(A.Data(), B.Data(), C.Data());
}

template<class T>
void Strassen<T>::Level22::SW(const T *A, const T *B, T *C)
{
#ifdef LEVEL_LOAD_TEST__P
    auto time_p = TIME();
#endif

    T a11 = A[0];
    T a12 = A[1];
    T a21 = A[2];
    T a22 = A[3];
    T b11 = B[0];
    T b12 = B[1];
    T b21 = B[2];
    T b22 = B[3];
    C[0] = a11 * b11 + a12 * b21;
    C[1] = a11 * b12 + a12 * b22;
    C[2] = a21 * b11 + a22 * b21;
    C[3] = a21 * b12 + a22 * b22;

#ifdef LEVEL_LOAD_TEST__P
    load += DURATION(time_p);
#endif
}

template<class T>
void Strassen<T>::LevelClassic::SW(const T *A, const T *B, T *C)
{
#ifdef LEVEL_LOAD_TEST__P
    auto time_p = TIME();
#endif

    for (i_type i = 0; i < n; ++i)
    {
        for (i_type j = 0; j < n; ++j)
        {
            T sum = 0;
            for (i_type k = 0; k < n; ++k)
            {
                sum += A[i * n + k] * B[k * n + j];
            }
            C[i * n + j] = sum;
        }
    }

#ifdef LEVEL_LOAD_TEST__P
    load += DURATION(time_p);
#endif
};

template<class T>
void Strassen<T>::LevelEven::SW(const T *A, const T *B, T *C)
{
#ifdef LEVEL_LOAD_TEST__P
    auto time_p = TIME();
#endif

    for (i_type i = 0, in = n; i < n; ++i, ++in)
    {
        i_type iadj = i * n;
        i_type iadj2 = i * n * 2;
        i_type inadj2 = in * n * 2;
        for (i_type j = 0, jn = n; j < n; ++j, ++jn)
        {
            T a11 = A[iadj2 + j];
            T a12 = A[iadj2 + jn];
            T a21 = A[inadj2 + j];
            T a22 = A[inadj2 + jn];
            i_type jadj = iadj + j;
            A11[jadj] = a11;
            A12[jadj] = a11 + a22;
            A22[jadj] = a22;
            S1[jadj] = a21 + a22;
            S2[jadj] = a11 + a12;
            S3[jadj] = a21 - a11;
            S4[jadj] = a12 - a22;
        }
    }

    for (i_type i = 0, in = n; i < n; ++i, ++in)
    {
        i_type iadj = i * n;
        i_type iadj2 = i * n * 2;
        i_type inadj2 = in * n * 2;
        for (i_type j = 0, jn = n; j < n; ++j, ++jn)
        {
            T b11 = B[iadj2 + j];
            T b12 = B[iadj2 + jn];
            T b21 = B[inadj2 + j];
            T b22 = B[inadj2 + jn];
            i_type jadj = iadj + j;
            B11[jadj] = b11;
            B21[jadj] = b11 + b22;
            B22[jadj] = b22;
            T1[jadj] = b12 - b22;
            T2[jadj] = b21 - b11;
            T3[jadj] = b11 + b12;
            T4[jadj] = b21 + b22;
        }
    }
    

#ifdef LEVEL_LOAD_TEST__P
    level_load[n].first += DURATION(time_p);
#endif

    next->SW(A12, B21, R1);
    next->SW(S1, B11, R2);
    next->SW(A11, T1, R3);
    next->SW(A22, T2, R4);
    next->SW(S2, B22, R5);
    next->SW(S3, T3, R6);
    next->SW(S4, T4, R7);

#ifdef LEVEL_LOAD_TEST__P
    time_p = TIME();
#endif

    for (i_type i = 0, in = n; i < n; ++i, ++in)
    {
        for (i_type j = 0, jn = n; j < n; ++j, ++jn)
        {
            T r1 = R1[i * n + j];
            T r2 = R2[i * n + j];
            T r3 = R3[i * n + j];
            T r4 = R4[i * n + j];
            T r5 = R5[i * n + j];
            C[i * n * 2 + j] = r1 + r4 -r5 + R7[i * n + j];
            C[i * n * 2 + jn] = r3 + r5;
            C[in * n * 2 + j] = r2 + r4;
            C[in * n * 2 + jn] = r1 + r3 - r2 + R6[i * n + j];
        }
    }

#ifdef LEVEL_LOAD_TEST__P
    level_load[n].second += DURATION(time_p);
#endif
}

template<class T>
void Strassen<T>::LevelOdd::SW(const T *A, const T *B, T *C)
{
#ifdef LEVEL_LOAD_TEST__P
    auto time_p = TIME();
#endif

    i_type limit = n * 2 - 1;
    for (i_type i = 0, in = n; i < n; ++i, ++in)
    {
        i_type iadj = i * n;
        i_type iadj2 = i * limit;
        i_type inadj2 = in * limit;
        for (i_type j = 0, jn = n; j < n; ++j, ++jn)
        {
            T a11 = A[iadj2 + j];
            T a12 = (jn == limit) ? 0 : A[iadj2 + jn];
            T a21 = (in == limit) ? 0 : A[inadj2 + j];
            T a22 = (in == limit || jn == limit) ? 0 : A[inadj2 + jn];
            i_type jadj = iadj + j;
            A11[jadj] = a11;
            A12[jadj] = a11 + a22;
            A22[jadj] = a22;
            S1[jadj] = a21 + a22;
            S2[jadj] = a11 + a12;
            S3[jadj] = a21 - a11;
            S4[jadj] = a12 - a22;
        }
    }
    for (i_type i = 0, in = n; i < n; ++i, ++in)
    {
        i_type iadj = i * n;
        i_type iadj2 = i * limit;
        i_type inadj2 = in * limit;
        for (i_type j = 0, jn = n; j < n; ++j, ++jn)
        {
            T b11 = B[iadj2 + j];
            T b12 = (jn == limit) ? 0 : B[iadj2 + jn];
            T b21 = (in == limit) ? 0 : B[inadj2 + j];
            T b22 = (in == limit || jn == limit) ? 0 : B[inadj2 + jn];
            i_type jadj = iadj + j;
            B11[jadj] = b11;
            B21[jadj] = b11 + b22;
            B22[jadj] = b22;
            T1[jadj] = b12 - b22;
            T2[jadj] = b21 - b11;
            T3[jadj] = b11 + b12;
            T4[jadj] = b21 + b22;
        }
    }

#ifdef LEVEL_LOAD_TEST__P
    level_load[n].first += DURATION(time_p);
#endif

    next->SW(A12, B21, R1);
    next->SW(S1, B11, R2);
    next->SW(A11, T1, R3);
    next->SW(A22, T2, R4);
    next->SW(S2, B22, R5);
    next->SW(S3, T3, R6);
    next->SW(S4, T4, R7);

#ifdef LEVEL_LOAD_TEST__P
    time_p = TIME();
#endif

    for (i_type i = 0, in = n; i < n; ++i, ++in)
    {
        i_type iadj = i * n;
        i_type ciadj = i * limit;
        i_type cinadj = in * limit;
        for (i_type j = 0, jn = n; j < n; ++j, ++jn)
        {
            T r1 = R1[iadj + j];
            T r2 = R2[iadj + j];
            T r3 = R3[iadj + j];
            T r4 = R4[iadj + j];
            T r5 = R5[iadj + j];
            C[ciadj + j] = r1 + r4 -r5 + R7[iadj + j];
            if (jn != limit) C[ciadj + jn] = r3 + r5;
            if (in != limit) C[cinadj + j] = r2 + r4;
            if (in != limit && jn != limit) C[cinadj + jn] = r1 + r3 - r2 + R6[iadj + j];
        }
    }

#ifdef LEVEL_LOAD_TEST__P
    level_load[n].second += DURATION(time_p);
#endif

}

template<class T>
Strassen<T>::PointerBase::PointerBase(i_type n)
    : n(n)
{
    A11 = new T[n * n];
    A12 = new T[n * n];
    A22 = new T[n * n];
    B11 = new T[n * n];
    B21 = new T[n * n];
    B22 = new T[n * n];
    S1 = new T[n * n];
    S2 = new T[n * n];
    S3 = new T[n * n];
    S4 = new T[n * n];
    T1 = new T[n * n];
    T2 = new T[n * n];
    T3 = new T[n * n];
    T4 = new T[n * n];
    R1 = new T[n * n];
    R2 = new T[n * n];
    R3 = new T[n * n];
    R4 = new T[n * n];
    R5 = new T[n * n];
    R6 = new T[n * n];
    R7 = new T[n * n];
}

template<class T>
Strassen<T>::PointerBase::~PointerBase()
{
    delete[] A11;
    delete[] A12;
    delete[] A22;
    delete[] B11;
    delete[] B21;
    delete[] B22;
    delete[] S1;
    delete[] S2;
    delete[] S3;
    delete[] S4;
    delete[] T1;
    delete[] T2;
    delete[] T3;
    delete[] T4;
    delete[] R1;
    delete[] R2;
    delete[] R3;
    delete[] R4;
    delete[] R5;
    delete[] R6;
    delete[] R7;
}

} // namespace maykitbo


#pragma once

#include "winograd.h"

#include <vector>
#include <thread>
#include <memory>

namespace maykitbo {


template<class T>
class WinogradP : public Winograd<T>
{
    using M = Matrix<T>;
    using i_type = typename M::i_type;
    using data_t = typename M::base;
    using BW = Winograd<T>;
    using typename BW::Level, typename BW::LevelEven, typename BW::LevelOdd,
            typename BW::LevelClassic, typename BW::Level22, typename BW::PointerBase;
    using BW::L_, BW::n_;

    public:
        WinogradP(i_type n, i_type odd_cap = 25, i_type winograd_cap = 17);
        void Execute(const M &A, const M &B, M &C);
        static void Mul(const M &A, const M &B, M &C,
                        i_type odd_cap = 25, i_type winograd_cap = 17);

    private:
        struct LevelParallelAdj;
        struct LevelParallelEven;
        // struct LevelClassicParallel;

        template<class L>
        void PushLevel(int n);
};

template<class T>
void WinogradP<T>::Mul(const M &A, const M &B, M &C, i_type odd_cap, i_type winograd_cap)
{
    i_type n = A.GetCols();
    if (n <= 1 || B.GetCols() != n || C.GetCols() != n ||
        A.GetRows() != n || B.GetRows() != n || C.GetRows() != n)
    {
        throw std::invalid_argument("Matrix size not match");
    }
    WinogradP<T> W(n, odd_cap, winograd_cap);
    W.Execute(A, B, C);
}

template<class T>
struct WinogradP<T>::LevelParallelEven final : public Level, public BW::PointerBase
{
    using BW::Level::next;
    using PB = PointerBase;
    using PB::n;
    using PB::A11, PB::A12, PB::A22, PB::B11, PB::B21, PB::B22,
        PB::S1, PB::S2, PB::S3, PB::S4, PB::T1, PB::T2, PB::T3, PB::T4,
        PB::R1, PB::R2, PB::R3, PB::R4, PB::R5, PB::R6, PB::R7;
    
    Level *next1, *next2, *next3, *next4, *next5, *next6;

    LevelParallelEven(i_type n) : PointerBase(n) {}
    void SW(const T *A, const T *B, T *C) override;
};

template<class T>
struct WinogradP<T>::LevelParallelAdj final : public Level, public BW::PointerBase
{
    using Level::next;
    using PB = PointerBase;
    using PB::n;
    using PB::A11, PB::A12, PB::A22, PB::B11, PB::B21, PB::B22,
        PB::S1, PB::S2, PB::S3, PB::S4, PB::T1, PB::T2, PB::T3, PB::T4,
        PB::R1, PB::R2, PB::R3, PB::R4, PB::R5, PB::R6, PB::R7;
    
    Level *next1, *next2, *next3, *next4, *next5, *next6;

    LevelParallelAdj(i_type n) : PointerBase(n) {}
    void SW(const T *A, const T *B, T *C) override;
};

// template<class T>
// struct WinogradP<T>::LevelClassicParallel final : public BW::Level
// {
//     i_type n;
//     void SW(const T *A, const T *B, T *C) override;
//     LevelClassicParallel(i_type n) : n(n) {}
// };


template<class T>
template<class L>
void WinogradP<T>::PushLevel(int n)
{
    for (i_type k = 0; k < EVIL; ++k)
    {
        L_.push_back(std::make_unique<L>(n));
    }
}

template<class T>
WinogradP<T>::WinogradP(i_type n, i_type odd_cap, i_type winograd_cap) : BW()
{
    if (n <= 1)
        throw std::invalid_argument("Matrix size must be greater than 1");

    n_ = n;
    bool odd = (n % 2 != 0);
    bool start_odd = odd;
    n += odd;
    bool cap = false;

    n /= 2;
    if ((odd && n < odd_cap) || n * 2 < winograd_cap)
    {
        L_.push_back(std::make_unique<LevelClassic>(n * 2 - odd));
        cap = true;
        return;
    }
    if (odd)
        L_.push_back(std::make_unique<LevelParallelAdj>(n));
    else
        L_.push_back(std::make_unique<LevelParallelEven>(n));

    odd = (n % 2 != 0);
    n += odd;

    while ((n /= 2) != 1) {
        if ((odd && n < odd_cap) || n * 2 < winograd_cap)
        {
            PushLevel<LevelClassic>(n * 2 - odd);
            cap = true;
            break;
        }
        if (odd)
            PushLevel<LevelOdd>(n);
        else
            PushLevel<LevelEven>(n);

        odd = (n % 2 != 0);
        n += odd;
    }

    if (!cap)
    {
        for (i_type k = 0; k < EVIL; ++k)
        {
            L_.push_back(std::make_unique<Level22>());
        }
    }

    Level *front = L_.front().get();
    front->next = &(*L_[1]);
    if (start_odd)
    {
        LevelParallelAdj *pol = static_cast<LevelParallelAdj*>(front);
        pol->next1 = &(*L_[2]);
        pol->next2 = &(*L_[3]);
        pol->next3 = &(*L_[4]);
        pol->next4 = &(*L_[5]);
        pol->next5 = &(*L_[6]);
        pol->next6 = &(*L_[7]);
    }
    else
    {
        LevelParallelEven *pel = static_cast<LevelParallelEven*>(front);
        pel->next1 = &(*L_[2]);
        pel->next2 = &(*L_[3]);
        pel->next3 = &(*L_[4]);
        pel->next4 = &(*L_[5]);
        pel->next5 = &(*L_[6]);
        pel->next6 = &(*L_[7]);
    }

    for (i_type i = 1; i < L_.size() - EVIL; ++i) {
        L_[i]->next = &(*L_[i + EVIL]);
    }
}


template<class T>
void WinogradP<T>::Execute(const M &A, const M &B, M &C)
{
    if (A.GetCols() != n_ || B.GetCols() != n_ || C.GetCols() != n_ ||
        A.GetRows() != n_ || B.GetRows() != n_ || C.GetRows() != n_) {
        throw std::invalid_argument("Matrix size not match");
    }
    L_[0]->SW(A.Data(), B.Data(), C.Data());
}

template<class T>
void WinogradP<T>::LevelParallelEven::SW(const T *A, const T *B, T *C)
{
    std::vector<std::thread> thrs;
    for (unsigned thc = 0; thc < 4; ++thc)
    {
        thrs.emplace_back([&, thc]
        {
            unsigned delta = n / 4;
            unsigned end = (thc == 3 ? n : (thc + 1) * delta);
            for (i_type i = thc * delta, in = n + thc * delta; i < end; ++i, ++in)
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
                    A12[jadj] = a12;
                    A22[jadj] = a22;
                    S1[jadj] = a21 + a22;
                    S2[jadj] = a21 + a22 - a11;
                    S3[jadj] = a11 - a21;
                    S4[jadj] = a12 - a21 - a22 + a11;
                }
            }
        });
    }
    for (unsigned thc = 0; thc < 4; ++thc)
    {
        thrs.emplace_back([&, thc]
        {
            unsigned delta = n / 4;
            unsigned end = (thc == 3 ? n : (thc + 1) * delta);
            for (i_type i = thc * delta, in = n + thc * delta; i < end; ++i, ++in)
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
                    B21[jadj] = b21;
                    B22[jadj] = b22;
                    T1[jadj] = b12 - b11;
                    T2[jadj] = b22 - b12 + b11;
                    T3[jadj] = b22 - b12;
                    T4[jadj] = b22 - b12 + b11 - b21;
                }
            }
        });
    }
    for (auto &i : thrs)
        i.join();

    thrs.clear();

    std::thread t1(&Level::SW, next, A11, B11, R1);
    std::thread t2(&Level::SW, next1, A12, B21, R2);
    std::thread t3(&Level::SW, next2, S4, B22, R3);
    std::thread t4(&Level::SW, next3, A22, T4, R4);
    std::thread t5(&Level::SW, next4, S1, T1, R5);
    std::thread t6(&Level::SW, next5, S2, T2, R6);
    std::thread t7(&Level::SW, next6, S3, T3, R7);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
    t7.join();

    for (unsigned thc = 0; thc < 4; ++thc)
    {
        thrs.emplace_back([&, thc]
        {
            unsigned delta = n / 4;
            unsigned end = (thc == 3 ? n : (thc + 1) * delta);
            for (i_type i = thc * delta, in = n + thc * delta; i < end; ++i, ++in)
            {
                for (i_type j = 0, jn = n; j < n; ++j, ++jn)
                {
                    T r1 = R1[i * n + j];
                    T r7 = R7[i * n + j];
                    T r16 = r1 + R6[i * n + j];
                    T r165 = r16 + R5[i * n + j];
                    C[i * n * 2 + j] = r1 + R2[i * n + j];
                    C[i * n * 2 + jn] = r165 + R3[i * n + j];
                    C[in * n * 2 + j] = r16 - R4[i * n + j] + r7;
                    C[in * n * 2 + jn] = r165 + r7;
                }
            }
        });
    }
    for (auto &i : thrs)
        i.join();

}

template<class T>
void WinogradP<T>::LevelParallelAdj::SW(const T *A, const T *B, T *C)
{

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
            A12[jadj] = a12;
            A22[jadj] = a22;
            S1[jadj] = a21 + a22;
            S2[jadj] = a21 + a22 - a11;
            S3[jadj] = a11 - a21;
            S4[jadj] = a12 - a21 - a22 + a11;
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
            B21[jadj] = b21;
            B22[jadj] = b22;
            T1[jadj] = b12 - b11;
            T2[jadj] = b22 - b12 + b11;
            T3[jadj] = b22 - b12;
            T4[jadj] = b22 - b12 + b11 - b21;
        }
    }

    std::thread t1(&Level::SW, next, A11, B11, R1);
    std::thread t2(&Level::SW, next1, A12, B21, R2);
    std::thread t3(&Level::SW, next2, S4, B22, R3);
    std::thread t4(&Level::SW, next3, A22, T4, R4);
    std::thread t5(&Level::SW, next4, S1, T1, R5);
    std::thread t6(&Level::SW, next5, S2, T2, R6);
    std::thread t7(&Level::SW, next6, S3, T3, R7);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
    t7.join();

    for (i_type i = 0, in = n; i < n; ++i, ++in)
    {
        i_type adj = i * n;
        i_type ciadj = i * limit;
        i_type cinadj = in * limit;
        for (i_type j = 0, jn = n; j < n; ++j, ++jn)
        {
            T r1 = R1[adj + j];
            T r7 = R7[adj + j];
            T r16 = r1 + R6[adj + j];
            T r165 = r16 + R5[adj + j];
            C[ciadj + j] = r1 + R2[adj + j];
            if (jn != limit) C[ciadj + jn] = r165 + R3[adj + j];
            if (in != limit) C[cinadj + j] = r16 - R4[adj + j] + r7;
            if (in != limit && jn != limit) C[cinadj + jn] = r165 + r7;
        }
    }  
}


} // namespace maykitbo


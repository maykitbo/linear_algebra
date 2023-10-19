#include "../utility/utility.h"


#define LEVEL_LOAD_TEST__P

#include "strassen_winograd/winograd.h"
#include "strassen_winograd/strassen.h"

#include <fstream>

using namespace maykitbo;

void ToFile(const std::string &name, const std::vector<std::pair<double, double>> &data) {
    std::ofstream out(name);
    for (auto &i : data) {
        out << i.first << " " << i.second << "\n";
    }
    out.close();
}

template<class T>
void F(const unsigned N, const unsigned repeat = 10) {
    using W = Winograd<T>;
    using S = Strassen<T>;
    W::load = 0;
    W::LevelClassic::load = 0;
    W::LevelEven::level_load.clear();
    W::LevelAdj::level_load.clear();
    S::load = 0;
    S::LevelClassic::load = 0;
    S::LevelEven::level_load.clear();
    S::LevelOdd::level_load.clear();

    Matrix<T> A(N, N, [] { return Random::Easy<T>::R(-100.0, 100.0); });
    Matrix<T> B(N, N, [] { return Random::Easy<T>::R(-100.0, 100.0); });
    Matrix<T> C(N, N);
    // for (unsigned k = 0; k < repeat; ++ k) {
    //     W::Mul(A, B, C);
    //     S::Mul(A, B, C);
    // }
    auto res = Time::Compare(repeat, [&] {
        W::Mul(A, B, C);
    }, [&] {
        S::Mul(A, B, C);
    });
    std::cout << N << ":\n";
    std::cout << "\tWinograd: " << Time::GetAdapt(res[0]) << "\n";
    std::cout << "\t\t" << Time::GetAdapt(W::load) << "\n\t\t"
                << Time::GetAdapt(W::LevelClassic::load) << "\n\t\t";
    for (auto &i : W::LevelEven::level_load) {
        std::cout << i.first << ":\t" << Time::GetAdapt(i.second.first) << "\t"
                    << Time::GetAdapt(i.second.second) << "\n\t\t";
    }
    for (auto &i : W::LevelAdj::level_load) {
        std::cout << i.first << ":\t" << Time::GetAdapt(i.second.first) << "\t"
                    << Time::GetAdapt(i.second.second) << "\n\t\t";
    }
    std::cout << "\n\tStrassen: " << Time::GetAdapt(res[1]) << "\n";
    std::cout << "\t\t" << Time::GetAdapt(S::load) << "\n\t\t"
                << Time::GetAdapt(S::LevelClassic::load) << "\n\t\t";
    for (auto &i : S::LevelEven::level_load) {
        std::cout << i.first << ":\t" << Time::GetAdapt(i.second.first) << "\t"
                    << Time::GetAdapt(i.second.second) << "\n\t\t";
    }
    for (auto &i : S::LevelOdd::level_load) {
        std::cout << i.first << ":\t" << Time::GetAdapt(i.second.first) << "\t"
                    << Time::GetAdapt(i.second.second) << "\n\t\t";
    }
    std::cout << "\n\n";
}

int main() {
    // F<float>(1024);
    // F<float>(2048);
    F<float>(3245, 1);
    // F<float>(1011);
    // F<float>(1012);
    // F<float>(1013);
    // F<float>(999);

    return 0;
}

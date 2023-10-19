
#pragma once

#include <chrono>
#include <functional>
#include <vector>
#include <string>

namespace maykitbo {

namespace Time {

typedef decltype((std::chrono::high_resolution_clock::now)()) TimeType;

// TimeType Now() { return std::chrono::high_resolution_clock::now(); }

typedef std::chrono::hours h;
typedef std::chrono::minutes min;
typedef std::chrono::seconds sec;
typedef std::chrono::milliseconds ms;
typedef std::chrono::microseconds mcs;
typedef std::chrono::nanoseconds ns;

template <class Unit = ms>
// int64_t Duration(T first, T second = Now()) {
int64_t Duration(TimeType first,
                 TimeType second = std::chrono::high_resolution_clock::now()) {
  return std::chrono::duration_cast<Unit>(second - first).count();
}

template <class Unit>
struct Prefix;

template <>
struct Prefix<h> {
  static constexpr const char val[] = "h";
  // static constexpr const unsigned short factor = 24;
};

template <>
struct Prefix<min> {
  static constexpr const char val[] = "min";
  // static constexpr const unsigned short factor = 60;
};

template <>
struct Prefix<sec> {
  static constexpr const char val[] = "sec";
  // static constexpr const unsigned short int factor = 60;
};

template <>
struct Prefix<ms> {
  static constexpr const char val[] = "ms";
  // static constexpr const unsigned short factor = 1000;
};

template <>
struct Prefix<mcs> {
  static constexpr const char val[] = "mcs";
  // static constexpr const unsigned short factor = 1000;
};

template <>
struct Prefix<ns> {
  static constexpr const char val[] = "ns";
  // static constexpr const unsigned short factor = 1000;
};

template <class Unit = ms>
int64_t Test(std::function<void(void)> test_func, int N = 1) {
  TimeType time_point = std::chrono::high_resolution_clock::now();
  for (int k = 0; k < N; ++k) {
    test_func();
  }
  return Duration<Unit>(time_point) / (int64_t)N;
}

// template<typename... Args, typename =
// std::enable_if_t<std::conjunction_v<std::is_same<Args,
// std::function<void(void)>>...>>>
template <class Unit = ms, class... Args>
std::vector<int64_t> Compare(int N, Args... functions) {
  std::vector<int64_t> result(sizeof...(Args));
  int i = 0;
  ((result[i++] = Test<Unit>(functions, N)), ...);
  return result;
}

template <class Unit = ns>
std::string GetAdapt(int64_t time_unit, int64_t max_len = 10000) {
  static const std::vector<std::pair<std::string, int>> factors{
      {"ns", 1000}, {"mcs", 1000}, {"ms", 1000},
      {"sec", 60},  {"min", 60},   {"h", 24}};
  std::string result;
  auto iter = factors.begin();
  while (iter->first != Prefix<Unit>::val) {
    ++iter;
  }
  while (time_unit > max_len && iter != factors.end()) {
    time_unit /= iter->second;
    ++iter;
  }
  return std::to_string(time_unit) + " " + iter->first;
}

}  // namespace Time

}  // namespace maykitbo

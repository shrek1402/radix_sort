// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// http://www.viva64.com

#include <algorithm>
#include <deque>
#include <iostream>
#include <iterator>
#include <list>
#include <vector>
#include "CPUtime.h"

#ifndef _STD
#define _STD ::std::
#endif
_STD list<int> a;
namespace rdx {
template <typename _Ty>
union _Union_radix_data {
  _Ty data;
  uint8_t mas_data[sizeof(_Ty)];
};

template <class _InIt>
inline void radix_msd(_InIt _First, _InIt _Last) {}

template <class _InIt>
inline void radix_lsd(
    _InIt _First,
    _InIt _Last) {  // perform function for each element [_First, _Last)
  _Adl_verify_range(_First, _Last);  // TODO: reverse !!!
  auto _UFirst = _STD _Get_unwrapped(_First);  // TODO get unwrapped
  const auto _ULast = _STD _Get_unwrapped(_Last);
  using _V_type = typename _InIt::value_type;
  _Union_radix_data<_V_type> _T_uni;
  _STD vector<_Union_radix_data<_V_type>> _Radix_list;
  _STD vector<_STD vector<_Union_radix_data<_V_type>>> _Radix_vec(
      256);  // without uni more fast?
  for (; _UFirst != _ULast; ++_UFirst) {
    _T_uni.data = *_UFirst;
    _Radix_list.push_back(_T_uni);
  }
  for (size_t i = 0; i < sizeof(_V_type); ++i) {
    auto begin2 = getCPUTime();
    for (auto _LFirst = _Radix_list.begin(); _LFirst != _Radix_list.end();
         ++_LFirst) {
      _Radix_vec.at((*_LFirst).mas_data[i]).push_back(*_LFirst);
    }
    _Radix_list.clear();

    for (size_t i = 0; i < 256; i++) {
      _Radix_list.insert(_Radix_list.end(), _Radix_vec.at(i).begin(),
                         _Radix_vec.at(i).end());
      _Radix_vec.at(i).clear();
    }
  }
}
}  // namespace rdx

int main() {
  _STD cout << "\t\t\t\t===== 8 bayts =====" << _STD endl;
  for (size_t i = 100; i <= 10000000; i *= 10) {
    _STD vector<uint64_t> a, b;
    for (size_t j = 0; j < i; j++) {
      uint64_t temp = ((((uint64_t)rand() << 45) | ((uint64_t)rand() << 30) |
                        ((uint64_t)rand() << 15) | ((uint64_t)rand()))
                       << 3) |
                      (rand() & 7);

      a.push_back(temp);
      b.push_back(temp);
    }

    auto begin = getCPUTime();
    rdx::radix_lsd(a.begin(), a.end());
    auto end = getCPUTime();

    auto begin1 = getCPUTime();
    _STD sort(b.begin(), b.end());
    auto end1 = getCPUTime();

    _STD cout << "cardinality: " << i << "\tradix: " << end - begin
              << "\tstd: " << end1 - begin1
              << "\tradix : std = " << (end - begin) / (end1 - begin1)
              << _STD endl;
  }

  std::cout << std::endl << "\t\t\t\t===== 4 bayts =====" << std::endl;
  for (size_t i = 100; i <= 100000000; i *= 10) {
    std::vector<uint32_t> a, b;
    for (size_t j = 0; j < i; j++) {
      uint32_t temp = ((((uint32_t)rand() << 30) | ((uint32_t)rand() << 15) |
                        ((uint32_t)rand()))
                       << 2) |
                      (rand() & 3);

      a.push_back(temp);
      b.push_back(temp);
    }

    auto begin = getCPUTime();
    rdx::radix_lsd(a.begin(), a.end());
    auto end = getCPUTime();

    auto begin1 = getCPUTime();
    std::sort(b.begin(), b.end());
    auto end1 = getCPUTime();

    std::cout << "cardinality: " << i << "\tradix: " << end - begin
              << "\tstd: " << end1 - begin1
              << "\tradix : std = " << (end - begin) / (end1 - begin1)
              << std::endl;
  }

  std::cout << std::endl << "\t\t\t\t===== 2 bayts =====" << std::endl;
  for (size_t i = 100; i <= 100000000; i *= 10) {
    std::vector<uint16_t> a, b;
    for (size_t j = 0; j < i; j++) {
      uint16_t temp = (((uint16_t)rand()) << 1) | (rand() & 1);

      a.push_back(temp);
      b.push_back(temp);
    }

    auto begin = getCPUTime();
    rdx::radix_lsd(a.begin(), a.end());
    auto end = getCPUTime();

    auto begin1 = getCPUTime();
    std::sort(b.begin(), b.end());
    auto end1 = getCPUTime();

    std::cout << "cardinality: " << i << "\tradix: " << end - begin
              << "\tstd: " << end1 - begin1
              << "\tradix : std = " << (end - begin) / (end1 - begin1)
              << std::endl;
  }

  std::cout << std::endl << "\t\t\t\t===== 1 bayt =====" << std::endl;
  for (size_t i = 100; i <= 100000000000; i *= 10) {
    std::vector<uint8_t> a, b;
    for (size_t j = 0; j < i; j++) {
      uint8_t temp = (uint8_t)rand();

      a.push_back(temp);
      b.push_back(temp);
    }

    auto begin = getCPUTime();
    rdx::radix_lsd(a.begin(), a.end());
    auto end = getCPUTime();

    auto begin1 = getCPUTime();
    std::sort(b.begin(), b.end());
    auto end1 = getCPUTime();

    std::cout << "cardinality: " << i << "\tradix: " << end - begin
              << "\tstd: " << end1 - begin1
              << "\tradix : std = " << (end - begin) / (end1 - begin1)
              << std::endl;
  }
}

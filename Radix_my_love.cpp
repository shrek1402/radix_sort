// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// http://www.viva64.com
#define  _ITERATOR_DEBUG_LEVEL 0
#include <algorithm>
#include <deque>
#include <iostream>
#include <iterator>
#include <vector>
#include "CPUtime.h"

#ifndef _STD
#define _STD ::std::
#endif

namespace rdx {
template <typename _Ty>
union _Union_radix_data {
  _Ty data;
  uint8_t mas_data[sizeof(_Ty)];
};

template <class _InIt>
inline void radix_msd(_InIt _First, _InIt _Last) {}

template <class _InIt>
// perform function for each element [_First, _Last)
inline void radix_lsd(_InIt _First, _InIt _Last) {  
  using _V_type = typename _InIt::value_type;
  _Union_radix_data<_V_type> _T_uni;
  _STD _Adl_verify_range(_First, _Last);  // TODO: reverse !!!
  _V_type* _UFirst = _STD _Get_unwrapped(_First);  // TODO get unwrapped
  const _V_type* _ULast = _STD _Get_unwrapped(_Last);
  _STD vector<_Union_radix_data<_V_type>> _Radix_vec;
  _STD vector<_STD vector<_Union_radix_data<_V_type>>> _Radix_vec_tmp(256);  // without uni more fast?
  
  for (; _UFirst != _ULast; ++_UFirst) {
    _T_uni.data = *_UFirst;    
    _Radix_vec.emplace_back(_T_uni);
  }
  for (size_t i = 0; i < sizeof(_V_type); ++i) {
    _Union_radix_data<_V_type>* _LFirst = _STD _Get_unwrapped(_Radix_vec.begin());
    const _Union_radix_data<_V_type>* _LLast = _STD _Get_unwrapped(_Radix_vec.end());
    for (; _LFirst != _LLast; ++_LFirst) {
      _Radix_vec_tmp.at((*_LFirst).mas_data[i]).emplace_back(*_LFirst);
    }
    _Radix_vec.clear();

    for (size_t i = 0; i < 256; i++) {
      _Radix_vec.insert(_Radix_vec.end(), _Radix_vec_tmp[i].begin(),
                         _Radix_vec_tmp[i].end());
      _Radix_vec_tmp[i].clear();
    }
  }
}
}  // namespace rdx
#define MAX_LENGTH 100000000// 2  1 1 0.9

int main() {
  _STD vector<uint64_t> random_vec;
  for (size_t i = 0; i < MAX_LENGTH; i++) {
    uint64_t temp =
        ((((uint64_t)rand() << 45) | ((uint64_t)rand() << 30) |
          ((uint64_t)rand() << 15) | ((uint64_t)rand()))
         << 3) |
        (rand() & 7);
    random_vec.emplace_back(temp);
  }

  _STD cout << "\t\t\t\t===== 8 byte =====" << _STD endl;
  for (size_t i = 100; i <= MAX_LENGTH; i *= 10) {
    _STD vector<uint64_t> a(i), b(i);
    _STD copy(random_vec.begin(), random_vec.begin() + i, a.begin());
    _STD copy(random_vec.begin(), random_vec.begin() + i, b.begin());

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
  return 0;
}

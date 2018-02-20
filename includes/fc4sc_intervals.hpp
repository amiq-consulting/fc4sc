#pragma once

#include "fc4sc_bin.hpp"

using std::max;
using std::make_pair;
using std::min;

namespace fc4sc {

  template <typename T>
  static vector<pair<T,T>> reunion(const vector<pair<T,T>>& lhs, const vector<pair<T,T>>& rhs) {

    vector<pair<T,T>> new_bins = lhs;
    new_bins.insert(new_bins.end() ,rhs.begin(), rhs.end());

    std::sort(new_bins.begin(), new_bins.end(), [](const pair<T,T>& lhs, const pair<T,T>& rhs) {
      return lhs.first < rhs.first;
    }); 

    vector<pair<T,T>> result_bins;
    result_bins.push_back(new_bins[0]);


    for (uint i = 1; i < new_bins.size(); ++i) {

      if(result_bins.back().second < new_bins[i].first)
        result_bins.push_back(new_bins[i]); 
      else 
        result_bins.back().second = std::max(result_bins.back().second, new_bins[i].second);

    }

    return result_bins;
  }

  template <typename T>
  static vector<pair<T,T>> intersection(const vector<pair<T,T>>& lhs, const vector<pair<T,T>>& rhs) {

    auto disjoint_lhs = reunion(lhs, {});
    auto disjoint_rhs = reunion(rhs, {});

    vector<pair<T,T>> results;

    for (auto &it_lhs: disjoint_lhs) {
      for (auto &it_rhs: disjoint_rhs) {
        
        if (it_lhs.second < it_rhs.first)
          continue;

        if (it_rhs.second < it_lhs.first)
          continue;


        auto new_intv = make_pair( max(it_lhs.first, it_rhs.first), min(it_rhs.second, it_lhs.second)   ) ;
 
        if (new_intv.first > new_intv.second)
          std::swap(new_intv.first, new_intv.second);

        results.push_back(new_intv);
      }
    }
    return results;

  }

  template <typename T>
  static vector<pair<T,T>> reunion(const bin<T>& lhs, const vector<pair<T,T>>& rhs) {
    return reunion(lhs.intervals, rhs);
  }

  template <typename T>
  static vector<pair<T,T>> reunion(const bin<T>& lhs, const bin<T>& rhs) {
    return reunion(lhs.intervals, rhs.intervals);
  }

  template <typename T>
  static vector<pair<T,T>> intersection(const bin<T>& lhs, const vector<pair<T,T>>& rhs) {
    return intersection(lhs.intervals, rhs);
  }

  template <typename T>
  static vector<pair<T,T>> intersection(const bin<T>& lhs, const bin<T>& rhs) {
    return intersection(lhs.intervals, rhs.intervals);
  }
  

}
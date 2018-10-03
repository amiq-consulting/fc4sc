/******************************************************************************

   Copyright 2003-2018 AMIQ Consulting s.r.l.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

******************************************************************************/
/******************************************************************************
   Original Authors: Teodor Vasilache and Dragos Dospinescu,
                     AMIQ Consulting s.r.l. (contributors@amiq.com)

               Date: 2018-Feb-20
******************************************************************************/

#ifndef FC4SC_INTERVAL_HPP
#define FC4SC_INTERVAL_HPP

#include "fc4sc_bin.hpp"

namespace fc4sc {

  template <typename T>
  static std::vector<interval_t<T>> reunion(const std::vector<interval_t<T>>& lhs,
					    const std::vector<interval_t<T>>& rhs) {

    std::vector<interval_t<T>> new_bins = lhs;
    new_bins.insert(new_bins.end() ,rhs.begin(), rhs.end());

    std::sort(new_bins.begin(), new_bins.end(), [](const interval_t<T>& lhs, const interval_t<T>& rhs) {
      return lhs.first < rhs.first;
    }); 

    std::vector<interval_t<T>> result_bins;
    result_bins.push_back(new_bins[0]);


    for (size_t i = 1; i < new_bins.size(); ++i) {

      if(result_bins.back().second < new_bins[i].first)
        result_bins.push_back(new_bins[i]); 
      else 
        result_bins.back().second = std::max(result_bins.back().second, new_bins[i].second);

    }

    return result_bins;
  }

  template <typename T>
  static std::vector<interval_t<T>> intersection(const std::vector<interval_t<T>>& lhs,
					    const std::vector<interval_t<T>>& rhs) {

    auto disjoint_lhs = reunion(lhs, {});
    auto disjoint_rhs = reunion(rhs, {});

    std::vector<interval_t<T>> results;

    for (auto &it_lhs: disjoint_lhs) {
      for (auto &it_rhs: disjoint_rhs) {
        
        if (it_lhs.second < it_rhs.first)
          continue;

        if (it_rhs.second < it_lhs.first)
          continue;


        auto new_intv = interval( max(it_lhs.first, it_rhs.first), min(it_rhs.second, it_lhs.second)   ) ;
 
        if (new_intv.first > new_intv.second)
          std::swap(new_intv.first, new_intv.second);

        results.push_back(new_intv);
      }
    }
    return results;

  }

  template <typename T>
  static std::vector<interval_t<T>> reunion(const bin<T>& lhs, const std::vector<interval_t<T>>& rhs) {
    return reunion(lhs.intervals, rhs);
  }

  template <typename T>
  static std::vector<interval_t<T>> reunion(const bin<T>& lhs, const bin<T>& rhs) {
    return reunion(lhs.intervals, rhs.intervals);
  }

  template <typename T>
  static std::vector<interval_t<T>> intersection(const bin<T>& lhs, const std::vector<interval_t<T>>& rhs) {
    return intersection(lhs.intervals, rhs);
  }

  template <typename T>
  static std::vector<interval_t<T>> intersection(const bin<T>& lhs, const bin<T>& rhs) {
    return intersection(lhs.intervals, rhs.intervals);
  }

}

#endif

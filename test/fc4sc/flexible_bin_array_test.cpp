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

               Date: 2018-Oct-29
******************************************************************************/

#include "fc4sc.hpp"
#include "gtest/gtest.h"
#include <utility> // for std::swap

auto increasing_intervals = [](size_t start, size_t N, size_t step) -> std::vector<fc4sc::interval_t<int>> {
  // resulting vector containing all
  std::vector<fc4sc::interval_t<int>> result;
  size_t orig_step = step;
  size_t last_start = start;
  for (size_t i = 0; i < N; i++) {
      result.push_back(interval(last_start, last_start + (i+1)*step));
      last_start = last_start + (i+1)*step + 1;
      step += orig_step;
  }
  return result;
};

auto fibonacci = [](size_t N) -> std::vector<int> {
  // start with 1 and 2, so that we don't have repeating elements
  int f0 = 1, f1 = 2;
  // resulting vector containing all
  std::vector<int> result(N, f0);

  for (size_t i = 1; i < N; i++) {
    std::swap(f0, f1);
    result[i] = f0;
    f1 += f0;
  }
  return result;
};

class flexible_bin_array_cvg: public covergroup {
private:
  int val = 0;

public:
  CG_CONS(flexible_bin_array_cvg) {};
  COVERPOINT(int, bin_array_cvp, val) {
    // this bin_array contains 5 different bins for values:
    // 1, 2, 3, 5, 8
    bin_array<int>("fibonacci", fibonacci(5)),
    bin_array<int>("intervals", increasing_intervals(0, 5, 3))
  };

  void sample(int val) {
    this->val = val; sample();
  }
};

TEST(flexible_bin_array, base) {
  // how much each bin weights in the overall coverage of the covergroup
  double bin_hit_weight = (100/10); // total 10 bins

  flexible_bin_array_cvg cvg;
  EXPECT_EQ(cvg.get_inst_coverage(), 0);

  cvg.sample(8);  // hits 2 bins
  EXPECT_EQ(cvg.get_inst_coverage(), 2 * bin_hit_weight);

  cvg.sample(18); // hits 1 bin
  EXPECT_EQ(cvg.get_inst_coverage(), 3 * bin_hit_weight);

  cvg.sample(3);  // hits 2 bins
  EXPECT_EQ(cvg.get_inst_coverage(), 5 * bin_hit_weight);
}

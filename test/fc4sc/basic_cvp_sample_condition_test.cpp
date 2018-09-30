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

               Date: 2018-Sep-24
******************************************************************************/

#include "fc4sc.hpp"
#include "gtest/gtest.h"

class cvp_sample_cond_test : public covergroup {
public:
  CG_CONS(cvp_sample_cond_test) {
  };

  int value = 0;

  COVERPOINT(int, never_sample_cvp, value, false) {
    illegal_bin<int>("illegal_3", 3),
    ignore_bin<int>("ignore_2", 2),
    bin<int>("four", 4)
  };

  COVERPOINT(int, odd_values_cvp, value, value % 2 == 1) {
    bin_array<int>("1_to_10", 10, interval(1, 10))
  };
};

TEST(cvp_sample, sample_condition) {
  cvp_sample_cond_test cvg;

  EXPECT_EQ(cvg.get_inst_coverage(), 0);

  for (auto i: {1,2,3,4,5,6,7,8,9,10}) {
      EXPECT_EQ(cvg.odd_values_cvp.get_inst_coverage(), (i / 2) * 10);
      EXPECT_EQ(cvg.never_sample_cvp.get_inst_coverage(), 0);
      EXPECT_EQ(cvg.get_inst_coverage(), cvg.odd_values_cvp.get_inst_coverage() / 2);
      cvg.value = i;
      cvg.sample(); // default sample function
  }

  EXPECT_EQ(cvg.odd_values_cvp.get_inst_coverage(), 50);
  EXPECT_EQ(cvg.never_sample_cvp.get_inst_coverage(), 0);
  EXPECT_EQ(cvg.get_inst_coverage(), 25);
}

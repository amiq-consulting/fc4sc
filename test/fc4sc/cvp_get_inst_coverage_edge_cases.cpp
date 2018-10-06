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

#include "fc4sc.hpp"
#include "gtest/gtest.h"

class cvp_0_weight_test : public covergroup {
private:
  int sample_point = 0;

public:
  CG_CONS(cvp_0_weight_test) {
    cvp_weight_0.option.weight = 0;
  };

  void sample(const int& x) {
    this->sample_point = x;
    sample();
  }

  COVERPOINT(int, cvp_weight_0, sample_point) {
    bin<int>("1", 1)
  };
};


TEST(cvp_get_inst_coverage, all_zero_weights) {
  cvp_0_weight_test cvg;

  EXPECT_EQ(cvg.cvp_weight_0.get_inst_coverage(), 0);
  
  int hit = -1, total = -1;
  EXPECT_EQ(cvg.cvp_weight_0.get_inst_coverage(hit, total), 0);
  EXPECT_EQ(hit, 0);
  EXPECT_EQ(total, 1);

  cvg.sample(1);

  EXPECT_EQ(cvg.cvp_weight_0.get_inst_coverage(), 100);
  
  hit = -1; total = -1;
  EXPECT_EQ(cvg.cvp_weight_0.get_inst_coverage(hit, total), 100);
  EXPECT_EQ(hit, 1);
  EXPECT_EQ(total, 1);

  // fc4sc::global_access::coverage_save("cvp_get_inst_coverage_" + std::string(::testing::UnitTest::GetInstance()->current_test_info()->name()) + ".xml");
}


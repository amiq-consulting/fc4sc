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
   Original Authors: Dragos Dospinescu,
                     AMIQ Consulting s.r.l. (contributors@amiq.com)

               Date: 2019-Nov-14
******************************************************************************/

#include "fc4sc.hpp"
#include "gtest/gtest.h"

class cvp_get_bin_hit_count_test : public covergroup {
private:
  int sample_point = 0;

public:
  CG_CONS(cvp_get_bin_hit_count_test) {
  };

  void sample() {
    covergroup::sample();
    this->sample_point++;
  }

  COVERPOINT(int, cvp_example, sample_point) {
    bin<int>("1,3,5", 1, 3, 5)
  };
};


TEST(cvp_get_bin_hit_count_test, basic) {
  cvp_get_bin_hit_count_test cvg;

  // nitially, hit count should be 0
  EXPECT_EQ(uint32_t(cvg.cvp_example.get_bin_hit_count(0)), 0);
  EXPECT_EQ(uint32_t(cvg.cvp_example.get_bin_hit_count(0)), 0);

  // hits should increase every 2 samples
  cvg.sample();
  EXPECT_EQ(uint32_t(cvg.cvp_example.get_bin_hit_count(0)), 0);
  cvg.sample();
  EXPECT_EQ(uint32_t(cvg.cvp_example.get_bin_hit_count(0)), 1);
  cvg.sample();
  EXPECT_EQ(uint32_t(cvg.cvp_example.get_bin_hit_count(0)), 1);
  cvg.sample();
  EXPECT_EQ(uint32_t(cvg.cvp_example.get_bin_hit_count(0)), 2);
  cvg.sample();
  EXPECT_EQ(uint32_t(cvg.cvp_example.get_bin_hit_count(0)), 2);
  cvg.sample();
  EXPECT_EQ(uint32_t(cvg.cvp_example.get_bin_hit_count(0)), 3);
}

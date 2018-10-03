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

int factorial(uint32_t n) {
    return (n == 1 || n == 0) ? 1 : n*factorial(n-1);
}

class cvp_sample_test : public covergroup {
public:
  CG_CONS(cvp_sample_test) {};

  int value = 0;

  void sample(const int& x) {
    this->value = x;
    covergroup::sample();
  }

  COVERPOINT(int, factorial_cvp, factorial(value)) {
    bin<int>("fact(3)", 6),  // value == 3
    bin<int>("fact(4)", 24)  // value == 4
  };

  COVERPOINT(int, square_value_cvp, value * value) {
    bin<int>("4^2", 16) // value == 4
  };
};

TEST(cvp_sample, sample_expression) {
  cvp_sample_test cvg;
  EXPECT_EQ(cvg.get_inst_coverage(), 0);

  cvg.value = 3;
  cvg.sample(); // default sample function

  EXPECT_EQ(cvg.factorial_cvp.get_inst_coverage(), 50);
  EXPECT_EQ(cvg.square_value_cvp.get_inst_coverage(), 0);
  EXPECT_EQ(cvg.get_inst_coverage(), 25);

  cvg.sample(4); // custom sample function
  EXPECT_EQ(cvg.factorial_cvp.get_inst_coverage(), 100);
  EXPECT_EQ(cvg.square_value_cvp.get_inst_coverage(), 100);
  EXPECT_EQ(cvg.get_inst_coverage(), 100);
}

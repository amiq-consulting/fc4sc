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

class cvg_illegal_test : public covergroup {
public:

  CG_CONS(cvg_illegal_test) {
  };

  // Must define sample
  int SAMPLE_POINT(sample_point, cvp1);

  void sample(const int& x) {
    this->sample_point = x;
    covergroup::sample();
  }

  coverpoint<int> cvp1 = coverpoint<int> (this, "cvp1",
    bin<int>("1", 1),
    illegal_bin<int>("illegal_2", 2)
  );

};


TEST(illegal_bin, sample_throws) {

  cvg_illegal_test cvg;
  EXPECT_EQ(cvg.get_inst_coverage(), 0);

  cvg.sample(1);
  EXPECT_EQ(cvg.get_inst_coverage(), 100);
  
  ASSERT_ANY_THROW(cvg.sample(2));	
  
  EXPECT_EQ(cvg.get_inst_coverage(), 100);
  
}

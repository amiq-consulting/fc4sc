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

class cvg_control_test : public covergroup {
public:

  CG_CONS(cvg_control_test) {};

  int SAMPLE_POINT(sample_point1, cvp1);
  int SAMPLE_POINT(sample_point2, cvp2);

  void sample(const int& x) {
    this->sample_point1 = x;
    this->sample_point2 = x;
    covergroup::sample();
  }

  coverpoint<int> cvp1 = coverpoint<int> (this, bin<int>("1", 1));
  coverpoint<int> cvp2 = coverpoint<int> (this, bin<int>("2", 1));
};


TEST(api, control) {

  cvg_control_test cvg;

  EXPECT_EQ(cvg.get_inst_coverage(), 0);
  
  cvg.stop();
  cvg.sample(1);

  EXPECT_EQ(cvg.get_inst_coverage(), 0);

  cvg.start();
  cvg.cvp1.stop();
  cvg.sample(1);

  EXPECT_EQ(cvg.cvp1.get_inst_coverage(), 0);
  EXPECT_EQ(cvg.cvp2.get_inst_coverage(), 100);  
  EXPECT_EQ(cvg.get_inst_coverage(), 50);

  cvg.cvp1.start();
  cvg.sample(1);
  
  EXPECT_EQ(cvg.cvp1.get_inst_coverage(), 100);
  EXPECT_EQ(cvg.cvp2.get_inst_coverage(), 100);  
  EXPECT_EQ(cvg.get_inst_coverage(), 100);

}

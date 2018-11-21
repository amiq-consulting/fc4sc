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

class basic_cross_options_test : public covergroup {
public:

  CG_CONS(basic_cross_options_test) {
    cvp1.option.weight = 0;
    cvp2.option.weight = 0;
  };

  int SAMPLE_POINT(sample_point_1,cvp1);
  int SAMPLE_POINT(sample_point_2,cvp2);

  void sample(const int& x, const int& y) {
    this->sample_point_1 = x;
    this->sample_point_2 = y;

    covergroup::sample();
  }

  coverpoint<int> cvp1 = coverpoint<int> (this, "cvp1",
    bin<int>("1", 1),
    bin<int>("2", 2)
  );

  coverpoint<int> cvp2 = coverpoint<int> (this, "cvp2",
    bin<int>("1", 1),
    bin<int>("2", 2)
  );

  cross<int,int> cross1 = cross<int,int> (this, "cross1",
    &cvp1,
    &cvp2
  );

};


TEST(cross_option, goal) {

  basic_cross_options_test basic_cg_1;
  

  EXPECT_EQ(basic_cg_1.get_inst_coverage(), 0);

  basic_cg_1.sample(0,0);  
  EXPECT_EQ(basic_cg_1.get_inst_coverage(), 0);

  basic_cg_1.sample(1,0);  
  EXPECT_EQ(basic_cg_1.get_inst_coverage(), 0);

  basic_cg_1.sample(0,1);  
  EXPECT_EQ(basic_cg_1.get_inst_coverage(), 0);

  basic_cg_1.sample(1,2);  
  EXPECT_EQ(basic_cg_1.get_inst_coverage(), 25);

  basic_cg_1.sample(1,2);  
  EXPECT_EQ(basic_cg_1.get_inst_coverage(), 25);

  basic_cg_1.cross1.option.goal = 25;
  EXPECT_EQ(basic_cg_1.cross1.get_inst_coverage(), 100);
  EXPECT_EQ(basic_cg_1.get_inst_coverage(), 100);

  basic_cg_1.cross1.option.goal = 100;

  basic_cg_1.sample(2,2);  
  EXPECT_EQ(basic_cg_1.get_inst_coverage(), 50);

  basic_cg_1.sample(2,1);  
  EXPECT_EQ(basic_cg_1.get_inst_coverage(), 75);

  basic_cg_1.sample(1,1);  
  EXPECT_EQ(basic_cg_1.get_inst_coverage(), 100);

}


TEST(cross_option, weight) {

  basic_cross_options_test basic_cg_1;
  basic_cg_1.cvp1.option.weight = 1;
  basic_cg_1.cvp2.option.weight = 1;
  basic_cg_1.cross1.option.weight = 2;

  EXPECT_EQ(basic_cg_1.get_inst_coverage(), 0);

  basic_cg_1.sample(1,2);  
  EXPECT_EQ(basic_cg_1.get_inst_coverage(), 37.5);

  basic_cg_1.sample(2,2);  
  EXPECT_EQ(basic_cg_1.get_inst_coverage(), 62.5);

  basic_cg_1.sample(2,1);  
  EXPECT_EQ(basic_cg_1.get_inst_coverage(), 87.5);

  basic_cg_1.sample(1,1);  
  EXPECT_EQ(basic_cg_1.get_inst_coverage(), 100);

}


TEST(cross_option, at_least) {

  basic_cross_options_test basic_cg_1;
  basic_cg_1.cross1.option.at_least = 2;


  basic_cg_1.sample(1,2);  
  EXPECT_EQ(basic_cg_1.get_inst_coverage(), 0);
  basic_cg_1.sample(1,2);  
  EXPECT_EQ(basic_cg_1.get_inst_coverage(), 25);
  basic_cg_1.sample(1,2);  
  EXPECT_EQ(basic_cg_1.get_inst_coverage(), 25);

}

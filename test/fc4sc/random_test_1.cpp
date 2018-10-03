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

class basic_test_1 : covergroup {
public:
  CG_CONS(basic_test_1) {};

  // Must define sample
  int SAMPLE_POINT(sample_point1, cvp1);
  int SAMPLE_POINT(sample_point2, cvp2);

  void sample(const int& x) {
    this->sample_point1 = x;
    this->sample_point2 = x;
    covergroup::sample();
  }

  // coverpoint with 10 one value bins
  coverpoint<int> cvp1 = coverpoint<int> (this,
      bin<int>("1",1),
      bin<int>("2",2),
      bin<int>("4",4),
      bin<int>("8",8),
      bin<int>("16",16),
      bin<int>("32",32),
      bin<int>("64",64),
      bin<int>("128",128),
      bin<int>("256",256),
      bin<int>("512",512),
      bin<int>("1024",1024)
  );

  // Same bins duplicated
  coverpoint<int> cvp2 = coverpoint<int> (this,
     bin<int>("1",1),
     bin<int>("2",2),
     bin<int>("4",4),
     bin<int>("8",8),
     bin<int>("16",16),
     bin<int>("32",32),
     bin<int>("64",64),
     bin<int>("128",128),
     bin<int>("256",256),
     bin<int>("512",512),
     bin<int>("1024",1024)
  );
};

TEST(random, 1) {

  basic_test_1 basic_test_1_cg;

  double cov = fc4sc::global::get_coverage("basic_test_1");

  for(int i=1; i<1025; ++i) {
   basic_test_1_cg.sample(i);

   // i is power of 2

   if ((i & (i-1)) == 0) {

     // Assert coverage increases on each power of 2
   	EXPECT_GT(fc4sc::global::get_coverage("basic_test_1"), cov);
      cov = fc4sc::global::get_coverage("basic_test_1");

   }

   // If ! power of two, count shouldn't go up
   EXPECT_EQ(fc4sc::global::get_coverage("basic_test_1"), cov);

}

EXPECT_EQ(fc4sc::global::get_coverage("basic_test_1"), 100);
EXPECT_EQ(fc4sc::global::get_coverage(), 100);

}

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

TEST(intervals, reunion) {

  using std::vector;
  bin<int> a("1", interval(1,5), interval(7,7), interval(8,20) );
  bin<int> b("2", interval(2,3), interval(5,9) );
  bin<int> c("3", interval(1,3), interval(3,6), interval(6,9) );
  bin<int> d("4", interval(2,4), interval(4,6) , interval(6,8), interval(10,19) );

  vector<fc4sc::interval_t<int>> a_ = {interval(1,5), interval(7,7), interval(8,20) };
  vector<fc4sc::interval_t<int>> b_ = {interval(2,3), interval(5,9) };
  vector<fc4sc::interval_t<int>> c_ = {interval(1,9)};
  vector<fc4sc::interval_t<int>> d_ = {interval(2,8), interval(10,19)};

  vector<fc4sc::interval_t<int>> a_b = {interval(1,20)};

  auto a_self = reunion(a,a);
  auto b_self = reunion(b,b);
  auto c_self = reunion(c,c);
  auto d_self = reunion(d,d);

  auto a_b_self = reunion(a,b);
  auto c_d_self = reunion(c,d);

  ASSERT_EQ(a_.size(), a_self.size());
  for (uint i=0; i < a_.size(); ++i) {
    EXPECT_EQ(a_[i].first, a_self[i].first);
    EXPECT_EQ(a_[i].second, a_self[i].second);
  }

  ASSERT_EQ(b_.size(), b_self.size());
  for (uint i=0; i < b_.size(); ++i) {
    EXPECT_EQ(b_[i].first, b_self[i].first);
    EXPECT_EQ(b_[i].second, b_self[i].second);
  }

  ASSERT_EQ(c_.size(), c_self.size());
  for (uint i=0; i < c_.size(); ++i) {
    EXPECT_EQ(c_[i].first, c_self[i].first);
    EXPECT_EQ(c_[i].second, c_self[i].second);
  }

  ASSERT_EQ(d_.size(), d_self.size());
  for (uint i=0; i < d_.size(); ++i) {
    EXPECT_EQ(d_[i].first, d_self[i].first);
    EXPECT_EQ(d_[i].second, d_self[i].second);
  }


  ASSERT_EQ(a_b.size(), a_b_self.size());
  for (uint i=0; i < a_b.size(); ++i) {
    EXPECT_EQ(a_b[i].first, a_b_self[i].first);
    EXPECT_EQ(a_b[i].second, a_b_self[i].second);
  }


}

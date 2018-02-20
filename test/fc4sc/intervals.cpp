#include "../../includes/fc4sc.hpp"
#include "gtest/gtest.h"

TEST(intervals, reunion) {

  bin<int> a("1", make_pair(1,5), make_pair(7,7), make_pair(8,20) );
  bin<int> b("2", make_pair(2,3), make_pair(5,9) );
  bin<int> c("3", make_pair(1,3), make_pair(3,6), make_pair(6,9) );
  bin<int> d("4", make_pair(2,4), make_pair(4,6) , make_pair(6,8), make_pair(10,19) );

  vector<pair<int,int>> a_ = {make_pair(1,5), make_pair(7,7), make_pair(8,20) };
  vector<pair<int,int>> b_ = {make_pair(2,3), make_pair(5,9) };
  vector<pair<int,int>> c_ = {make_pair(1,9)};
  vector<pair<int,int>> d_ = {make_pair(2,8), make_pair(10,19)};

  vector<pair<int,int>> a_b = {make_pair(1,20)};

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
#include "../../includes/fc4sc.hpp"
#include "gtest/gtest.h"

class basic_cvp_goal_test : public covergroup {
public:

  // Must call UTIL somewhere in constructors to register new cvg
//  basic_cvp_weight_test() : UTIL(basic_cvp_weight_test) {
  CG_CONS(basic_cvp_goal_test) {
    cvp1.option.goal = 50;
    cvp2.option.goal = 100;
  };

  
  // Must define sample
  int SAMPLE_POINT(sample_point1,cvp1);
  int SAMPLE_POINT(sample_point2,cvp2);

  void sample(const int& x) {
    this->sample_point1 = x;
    this->sample_point2 = x;

    covergroup::sample();
  }

  // coverpoint with 10 one value bins
  coverpoint<int> cvp1 = coverpoint<int> (this,
    bin<int>(1),
    bin<int>(2)
  );

  coverpoint<int> cvp2 = coverpoint<int> (this,
    bin<int>(1),
    bin<int>(2)
  );

};

TEST(cvp_options, goal) {

  basic_cvp_goal_test basic_cg;
  int i = 1;

  EXPECT_TRUE(basic_cg.get_coverage() == 0);

  basic_cg.sample(i);

  EXPECT_EQ(basic_cg.cvp1.get_inst_coverage(), 100);
  EXPECT_EQ(basic_cg.cvp2.get_inst_coverage(), 50);

  EXPECT_EQ(basic_cg.get_coverage(),75);

  basic_cg.sample(++i);

  EXPECT_TRUE(basic_cg.get_coverage() == 100);

}

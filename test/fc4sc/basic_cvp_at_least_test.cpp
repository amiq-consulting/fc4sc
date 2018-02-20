#include "../../includes/fc4sc.hpp"
#include "gtest/gtest.h"


class basic_cvp_at_least_test : public covergroup {
public:

  CG_CONS(basic_cvp_at_least_test) {
    cvp1.option.at_least = 1;
    cvp2.option.at_least = 2;
  }

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
    bin<int>(1)
  );

  coverpoint<int> cvp2 = coverpoint<int> (this, 
    bin<int>(1)
  );

};

TEST(cvp_options, at_least) {

  basic_cvp_at_least_test basic_cg;
  int i = 1;

  EXPECT_TRUE(basic_cg.get_coverage() == 0);

  basic_cg.sample(i);

  EXPECT_TRUE(basic_cg.get_coverage() == 50);

  basic_cg.sample(i);

  EXPECT_TRUE(basic_cg.get_coverage() == 100);

}


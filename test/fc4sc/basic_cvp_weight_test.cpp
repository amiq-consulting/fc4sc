#include "fc4sc.hpp"
#include "gtest/gtest.h"

class basic_cvp_weight_test : public covergroup {
public:

  CG_CONS(basic_cvp_weight_test) {
    cvp1.option.weight = 1;
    cvp2.option.weight = 2;
  };

  
  // Must define sample
  int SAMPLE_POINT(sample_point1,cvp1);
  int SAMPLE_POINT(sample_point2,cvp2);

  void sample(const int& x) {
    this->sample_point1 = x;
    this->sample_point2 = x;

    covergroup::sample();
  }

  coverpoint<int> cvp1 = coverpoint<int> (this,
    bin<int>(1)
  );

  coverpoint<int> cvp2 = coverpoint<int> (this,
    bin<int>(2)
  );

};


TEST(cvp_options, weight) {

  basic_cvp_weight_test basic_cg;
  int i = 1;

  EXPECT_TRUE(basic_cg.get_coverage() == 0);

  basic_cg.sample(i);

  EXPECT_TRUE(basic_cg.get_coverage() == (100.0 / 3));

  basic_cg.sample(++i);

  EXPECT_TRUE(basic_cg.get_coverage() == 100);

}

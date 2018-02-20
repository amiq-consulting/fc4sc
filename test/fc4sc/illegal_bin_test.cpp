#include "../../includes/fc4sc.hpp"
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
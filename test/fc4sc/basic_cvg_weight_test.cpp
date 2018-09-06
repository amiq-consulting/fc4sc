#include "fc4sc.hpp"
#include "gtest/gtest.h"

class basic_cvg_weight_test : public covergroup {
public:

  CG_CONS(basic_cvg_weight_test) {
  };

  int SAMPLE_POINT(sample_point, cvp1);

  void sample(const int& x) {
    this->sample_point = x;
    
    covergroup::sample();

  }

  coverpoint<int> cvp1 = coverpoint<int> (this,
    bin<int>(1)
  );

};


TEST(cvg_options, weight) {

  basic_cvg_weight_test basic_cg_1;
  basic_cvg_weight_test basic_cg_2;

  basic_cg_1.option.weight = 1;
  basic_cg_2.option.weight = 2;

  EXPECT_TRUE(basic_cg_1.get_coverage() == 0);
  EXPECT_TRUE(basic_cg_2.get_coverage() == 0);
  
  basic_cg_1.sample(1);

  EXPECT_EQ(basic_cg_1.get_inst_coverage(), 100);
  EXPECT_EQ(basic_cg_2.get_inst_coverage(), 0);
  EXPECT_EQ(basic_cg_1.get_coverage(), (100.0 / 3));
  EXPECT_EQ(basic_cg_1.get_coverage(), basic_cg_2.get_coverage());
  
  basic_cg_2.sample(1);

  EXPECT_EQ(basic_cg_1.get_inst_coverage(), 100);
  EXPECT_EQ(basic_cg_2.get_inst_coverage(), 100);
  EXPECT_EQ(basic_cg_1.get_coverage(), 100);
  EXPECT_EQ(basic_cg_1.get_coverage(), basic_cg_2.get_coverage());

}

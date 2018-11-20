#include "fc4sc.hpp"
#include "gtest/gtest.h"

class basic_cross_bins_filtering_test: public covergroup {
public:
  CG_CONS(basic_cross_bins_filtering_test){
    cvp1.option.weight = 0;
    cvp2.option.weight = 0;
  };

  int SAMPLE_POINT(sample_point_1, cvp1);
  int SAMPLE_POINT(sample_point_2, cvp2);

  void sample(const int& x, const int& y) {
    this->sample_point_1 = x;
    this->sample_point_2 = y;
    covergroup::sample();
  }
  coverpoint<int> cvp1 = coverpoint<int> (this, "cvp1",
    bin<int>("one", 1),
    bin<int>("two", 2)
  );
  coverpoint<int> cvp2 = coverpoint<int> (this, "cvp2",
    bin<int>("one", 1),
    bin<int>("two", 2)
  );
  cross<int,int> cross1 = cross<int,int> (this, "cross1", &cvp1, &cvp2);
};

TEST(cross_bins_filtering, binsof) {
    basic_cross_bins_filtering_test basic_cg_1;

    // TODO: update testcase to check the functionality of binsof, || and && operators
    EXPECT_EQ(basic_cg_1.get_inst_coverage(), 0);

    basic_cg_1.sample(0, 0);
    EXPECT_EQ(basic_cg_1.get_inst_coverage(), 0);

    basic_cg_1.sample(1, 0);
    EXPECT_EQ(basic_cg_1.get_inst_coverage(), 0);

    basic_cg_1.sample(0, 1);
    EXPECT_EQ(basic_cg_1.get_inst_coverage(), 0);

    basic_cg_1.sample(1, 2);
    EXPECT_EQ(basic_cg_1.get_inst_coverage(), 25);

    basic_cg_1.sample(1, 2);
    EXPECT_EQ(basic_cg_1.get_inst_coverage(), 25);

    basic_cg_1.cross1.option.goal = 25;
    EXPECT_EQ(basic_cg_1.cross1.get_inst_coverage(), 100);
    EXPECT_EQ(basic_cg_1.get_inst_coverage(), 100);

    basic_cg_1.cross1.option.goal = 100;

    basic_cg_1.sample(2, 2);
    EXPECT_EQ(basic_cg_1.get_inst_coverage(), 50);

    basic_cg_1.sample(2, 1);
    EXPECT_EQ(basic_cg_1.get_inst_coverage(), 75);

    basic_cg_1.sample(1, 1);
    EXPECT_EQ(basic_cg_1.get_inst_coverage(), 100);
}

#include "fc4sc.hpp"
#include "gtest/gtest.h"

class cvp_0_weight_test : public covergroup {
public:

  CG_CONS(cvp_0_weight_test) {
    cvp_weight_0.option.weight = 0;
  };

  
  // Must define sample
  int SAMPLE_POINT(sample_point1,cvp_weight_0);
  int SAMPLE_POINT(sample_point2,cvp_empty);
  int SAMPLE_POINT(sample_point3,cvp_empty_bins);

  void sample(const int& x) {
    this->sample_point1 = x;
    this->sample_point2 = x;
    this->sample_point3 = x;

    covergroup::sample();
  }

  coverpoint<int> cvp_weight_0 = coverpoint<int> (this, 
    bin<int>(1)
  );

  coverpoint<int> cvp_empty = coverpoint<int> (this);

  coverpoint<int> cvp_empty_bins = coverpoint<int> (this,
    bin<int>(),
    bin<int>("name")
    );  

};


TEST(cvp_get_inst_coverage, all_zero_weights) {

  cvp_0_weight_test cvg;

  EXPECT_EQ(cvg.cvp_weight_0.get_inst_coverage(), 0);
  
  int hit = -1, total = -1;
  EXPECT_EQ(cvg.cvp_weight_0.get_inst_coverage(hit, total), 0);
  EXPECT_EQ(hit, 0);
  EXPECT_EQ(total, 1);

  cvg.sample(1);

  EXPECT_EQ(cvg.cvp_weight_0.get_inst_coverage(), 100);
  
  hit = -1; total = -1;
  EXPECT_EQ(cvg.cvp_weight_0.get_inst_coverage(hit, total), 100);
  EXPECT_EQ(hit, 1);
  EXPECT_EQ(total, 1);

  // fc4sc::global_access::coverage_save("cvp_get_inst_coverage_" + std::string(::testing::UnitTest::GetInstance()->current_test_info()->name()) + ".xml");


}

TEST(cvp_get_inst_coverage, empty_cvp) {

  cvp_0_weight_test cvg;

  EXPECT_EQ(cvg.cvp_empty.get_inst_coverage(), 0);
  
  int hit = -1, total = -1;
  EXPECT_EQ(cvg.cvp_empty.get_inst_coverage(hit, total), 0);
  EXPECT_EQ(hit, 0);
  EXPECT_EQ(total, 0);

  cvg.cvp_empty.option.weight = 0;

  EXPECT_EQ(cvg.cvp_empty.get_inst_coverage(), 100);
  
  hit = -1; total = -1;
  EXPECT_EQ(cvg.cvp_empty.get_inst_coverage(hit, total), 100);
  EXPECT_EQ(hit, 0);
  EXPECT_EQ(total, 0);

  // fc4sc::global_access::coverage_save("cvp_get_inst_coverage_" + std::string(::testing::UnitTest::GetInstance()->current_test_info()->name()) + ".xml");

}

TEST(cvp_get_inst_coverage, all_bins_empty_cvp) {

  cvp_0_weight_test cvg;

  EXPECT_EQ(cvg.cvp_empty_bins.get_inst_coverage(), 0);
  
  int hit = -1, total = -1;
  EXPECT_EQ(cvg.cvp_empty_bins.get_inst_coverage(hit, total), 0);
  EXPECT_EQ(hit, 0);
  EXPECT_EQ(total, 0);

  cvg.cvp_empty_bins.option.weight = 0;

  EXPECT_EQ(cvg.cvp_empty_bins.get_inst_coverage(), 100);
  
  hit = -1; total = -1;
  EXPECT_EQ(cvg.cvp_empty_bins.get_inst_coverage(hit, total), 100);
  EXPECT_EQ(hit, 0);
  EXPECT_EQ(total, 0);

  // fc4sc::global_access::coverage_save("cvp_get_inst_coverage_" + std::string(::testing::UnitTest::GetInstance()->current_test_info()->name()) + ".xml");
}

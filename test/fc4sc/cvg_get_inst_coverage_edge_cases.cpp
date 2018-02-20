#include "../../includes/fc4sc.hpp"
#include "gtest/gtest.h"

class cvg_0_weight_test : public covergroup {
public:

  CG_CONS(cvg_0_weight_test) {
    cvp1.option.weight = 0;
    cvp2.option.weight = 0;
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


TEST(cvg_get_inst_coverage, all_zero_weights) {

  cvg_0_weight_test cvg;
  

  EXPECT_EQ(cvg.get_inst_coverage(), 0);
  EXPECT_EQ(cvg.get_coverage(), 0);
  
  int hit = -1, total = -1;
  EXPECT_EQ(cvg.get_inst_coverage(hit, total), 0);
  EXPECT_EQ(hit, 0);
  EXPECT_EQ(total, 0);

  hit = -1; total = -1;
  EXPECT_EQ(cvg.get_coverage(hit, total), 0);
  EXPECT_EQ(hit, 0);
  EXPECT_EQ(total, 0);

  cvg.option.weight = 0;
  cvg.type_option().weight = 0;

  hit = -1;  total = -1;

  EXPECT_EQ(cvg.get_inst_coverage(), 100);  
  EXPECT_EQ(cvg.get_coverage(), 100);  

  EXPECT_EQ(cvg.get_inst_coverage(hit, total), 100);
  EXPECT_EQ(hit, 0);
  EXPECT_EQ(total, 0);

  hit = -1;  total = -1;
  EXPECT_EQ(cvg.get_coverage(hit, total), 100);
  EXPECT_EQ(hit, 0);
  EXPECT_EQ(total, 0);

  // fc4sc::global_access::coverage_save("cvg_get_inst_coverage_" + std::string(::testing::UnitTest::GetInstance()->current_test_info()->name()) + ".xml");

}



class cvg_0_empty_cvg_test : public covergroup {
public:

  CG_CONS(cvg_0_empty_cvg_test) {};

};

TEST(cvg_get_inst_coverage, empty_cvg) {

  cvg_0_empty_cvg_test cvg;

  EXPECT_EQ(cvg.get_inst_coverage(), 0);
  EXPECT_EQ(cvg.get_coverage(), 0);
  
  int hit = -1, total = -1;
  EXPECT_EQ(cvg.get_inst_coverage(hit, total), 0);
  EXPECT_EQ(hit, 0);
  EXPECT_EQ(total, 0);

  hit = -1; total = -1;
  EXPECT_EQ(cvg.get_coverage(hit, total), 0);
  EXPECT_EQ(hit, 0);
  EXPECT_EQ(total, 0);


  cvg.option.weight = 0;
  cvg.type_option().weight = 0;
  hit = -1;  total = -1;

  EXPECT_EQ(cvg.get_inst_coverage(), 100);  
  EXPECT_EQ(cvg.get_inst_coverage(hit, total), 100);
  EXPECT_EQ(hit, 0);
  EXPECT_EQ(total, 0);

  hit = -1;  total = -1;
  EXPECT_EQ(cvg.get_coverage(), 100);  
  EXPECT_EQ(cvg.get_coverage(hit, total), 100);
  EXPECT_EQ(hit, 0);
  EXPECT_EQ(total, 0);

  // fc4sc::global_access::coverage_save("cvg_get_inst_coverage_" + std::string(::testing::UnitTest::GetInstance()->current_test_info()->name()) + ".xml");

}

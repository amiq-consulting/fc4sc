#include "../../includes/fc4sc.hpp"
#include "gtest/gtest.h"

class bin_array_test : public covergroup {
public:

 // Must call UTIL somewhere in constructors to register new cvg
  CG_CONS(bin_array_test) {};

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
    bin_array<int>("bin_arr_1", 5, interval(1,5))
  );

  coverpoint<int> cvp2 = coverpoint<int> (this,
    bin_array<int>("bin_arr_2", 10, interval(1,5))
  );

};

TEST(bin_array, edge_cases) {

  bin_array_test cvg;

  //cvp1 should have 5 bins -> {1}, {2}, {3}, {4} and {5}
  cvg.cvp2.stop();

  int total = -1;
  int hit = -1;
  EXPECT_EQ(cvg.cvp1.get_inst_coverage(hit, total), 0);
  EXPECT_EQ(total, 5);
  EXPECT_EQ(hit, 0);

  cvg.sample(1);
  EXPECT_EQ(cvg.cvp1.get_inst_coverage(hit, total), 20);
  EXPECT_EQ(total, 5);
  EXPECT_EQ(hit, 1);

  cvg.sample(6);
  EXPECT_EQ(cvg.cvp1.get_inst_coverage(hit, total), 20);
  EXPECT_EQ(total, 5);
  EXPECT_EQ(hit, 1);

  cvg.sample(2);
  EXPECT_EQ(cvg.cvp1.get_inst_coverage(hit, total), 40);
  EXPECT_EQ(total, 5);
  EXPECT_EQ(hit, 2);

  cvg.sample(3);
  EXPECT_EQ(cvg.cvp1.get_inst_coverage(hit, total), 60);
  EXPECT_EQ(total, 5);
  EXPECT_EQ(hit, 3);

  cvg.sample(4);
  EXPECT_EQ(cvg.cvp1.get_inst_coverage(hit, total), 80);
  EXPECT_EQ(total, 5);
  EXPECT_EQ(hit, 4);

  cvg.sample(5);
  EXPECT_EQ(cvg.cvp1.get_inst_coverage(hit, total), 100);
  EXPECT_EQ(total, 5);
  EXPECT_EQ(hit, 5);

  // cvp2 should have 1 bin -> {[1:5]} since length (which is 10) > interval ( [1:5])
  cvg.cvp2.start();

  total = -1;
  hit = -1;

  EXPECT_EQ(cvg.cvp2.get_inst_coverage(hit,total), 0);
  EXPECT_EQ(total, 1);
  EXPECT_EQ(hit, 0);

  cvg.sample(5);
  EXPECT_EQ(cvg.cvp2.get_inst_coverage(hit,total), 100);
  EXPECT_EQ(total, 1);
  EXPECT_EQ(hit, 1);

}

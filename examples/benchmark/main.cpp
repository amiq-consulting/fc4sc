#include <iostream>
#include <array>

#include "fc4sc.hpp"

#include "test_type_1.hpp"

using std::array;


using fc4sc::reunion;
using fc4sc::intersection;


template <typename T>
void print_arr(vector<pair<T,T>> x) {
  for (auto it: x)
    cerr << "[" << it.first << "," << it.second << "] ";
  cerr << "\n";
}

int main() {

  array<test_coverage, 1> test_array;    

  test_array[0].sample(37,1,1);
  test_array[0].sample(1,1,1);
  test_array[0].sample(6,1,1);
  test_array[0].sample(7,1,1);
  test_array[0].sample(38,1,1);
  test_array[0].sample(50,1,1);
  test_array[0].sample(80,1,1);
  test_array[0].sample(100,1,1);

  fc4sc::global::coverage_save("results.xml");

}
/******************************************************************************

   Copyright 2003-2018 AMIQ Consulting s.r.l.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

******************************************************************************/
/******************************************************************************
   Original Authors: Teodor Vasilache and Dragos Dospinescu,
                     AMIQ Consulting s.r.l. (contributors@amiq.com)

               Date: 2018-Feb-20
******************************************************************************/

#include <iostream>
#include <array>

#include "fc4sc.hpp"
#include "test_type_1.hpp"

using std::array;

using fc4sc::reunion;
using fc4sc::intersection;

template <typename T>
void print_arr(std::vector<fc4sc::interval_t<T>> x) {
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

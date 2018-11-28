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
   Original Authors: Dragos Dospinescu,
                     AMIQ Consulting s.r.l. (contributors@amiq.com)

               Date: 2018-Feb-20
******************************************************************************/

#include "fc4sc.hpp"
#include "gtest/gtest.h"

class basic_coverpoint_definitions_cvg : public covergroup {
public:
  CG_CONS(basic_coverpoint_definitions_cvg) {};
  bool         var_bool         = 65;
  char         var_char         = 65;
  float        var_float        = 65;
  double       var_double       = 65;
  int          var_int          = 65;
  unsigned int var_unsigned_int = 65;
  long         var_long         = 65;

  COVERPOINT(bool        , cvp_bool        , var_bool        ) { bin<bool        >("65", 65) };
  COVERPOINT(char        , cvp_char        , var_char        ) { bin<char        >("65", 65) };
  COVERPOINT(float       , cvp_float       , var_float       ) { bin<float       >("65", 65) };
  COVERPOINT(double      , cvp_double      , var_double      ) { bin<double      >("65", 65) };
  COVERPOINT(int         , cvp_int         , var_int         ) { bin<int         >("65", 65) };
  COVERPOINT(unsigned int, cvp_unsigned_int, var_unsigned_int) { bin<unsigned int>("65", 65) };
  COVERPOINT(long        , cvp_long        , var_long        ) { bin<long        >("65", 65) };
};

TEST(coverpoint, control) {
  basic_coverpoint_definitions_cvg cvg("cvg");
  EXPECT_EQ(cvg.get_inst_coverage(), 0);
  cvg.sample();
  EXPECT_EQ(cvg.get_inst_coverage(), 100);
}

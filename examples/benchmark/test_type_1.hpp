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

#pragma once

#include "fc4sc.hpp"


/*!

 * \brief Macro to sample a coverpoint ar a cross.
 *
 *  This macro sets some strings on the first sample the passes the
 * chosen value to the coverpoint/cross specified
 */
class test_coverage : public covergroup {
public:
    // Must call parent constructor somewhere register a new cvg
    CG_CONS(test_coverage) {  }

    int SAMPLE_POINT(value,values_cvp);
    int SAMPLE_POINT(reset, reset_cvp);
    int SAMPLE_POINT(valid, input_valid_cvp);
    void sample(int value, bool reset, bool valid) {
      this->value = value;
      this->reset = reset;
      this->valid = valid;
      covergroup::sample();
    }

    coverpoint<int> values_cvp = coverpoint<int> (this,
        bin<int>("zero", 0),
        bin<int>("low1", interval(1,5)),
        bin_array<int>("middle_1", 3, interval(6,100)),
        bin<int>("max", 255)
    );
    coverpoint<int> reset_cvp = coverpoint<int> (this, 
        bin<int>("active", 1),
        bin<int>("disabled", 0)
    );
    coverpoint<int> input_valid_cvp = coverpoint<int> (this,
        bin<int>("active", 1),
        bin<int>("disabled", 0)
    );
    cross<int, int, int> reset_valid_cross = cross<int, int, int> (this,
        &reset_cvp,
        &input_valid_cvp,
        &values_cvp
    );
};

/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

 *****************************************************************************/

/*****************************************************************************
 
  display.h -- 
 
  Original Author: Rocco Jonack, Synopsys, Inc.

 *****************************************************************************/
 
/*****************************************************************************
 
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/
#include <climits>
#include "fc4sc.hpp"

#include <array>

SC_MODULE(display) {

  sc_in<bool>  output_data_ready;
  sc_in<int>   result;

  int i, tmp1;
  
  class output_coverage : public covergroup {

    int SAMPLE_POINT(value, data_ready_cvp);
    int SAMPLE_POINT(valid, output_valid_cvp);

    public:

      // Must call parent constructor somewhere register a new cvg
      CG_CONS(output_coverage) {
      }

      void sample(int value, bool valid) {
        this->value = value;
        this->valid = valid;

        covergroup::sample();
      }

      coverpoint<int> data_ready_cvp = coverpoint<int> (this,
          bin<int>("zero", 0),
          illegal_bin<int>("illegal_zero", 0),
          bin<int>("positive", interval(1,INT_MAX - 1)),
          bin<int>("negative", interval(-1,INT_MIN + 1))
      );

      coverpoint<int> output_valid_cvp = coverpoint<int> (this,
          bin<int>("valid", 1),
          bin<int>("invalid", 0)
      );


    };

    output_coverage out_cg;

  SC_CTOR(display)
    {
      SC_METHOD(entry);
      dont_initialize();
      sensitive << output_data_ready.pos();
      i = 0;
      tmp1 = 0;
    }

  void entry();
};


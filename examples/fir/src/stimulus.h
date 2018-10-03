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
 
  stimulus.h -- 
 
  Original Author: Rocco Jonack, Synopsys, Inc.
 
 *****************************************************************************/

 /*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here. 
      Name, Affiliation: Teodor Vasilache and Dragos Dospinescu,
                         AMIQ Consulting s.r.l. (contributors@amiq.com)
                   Date: 2018-Feb-20

  Description of Modification: Included the FC4SC library, created and 
  instantiated a covergroup for collecting stimulus data coverage.

  	  	  	  	   Date: 2018-Sep-24

  Description of Modification: Updated the instantiation of the coverpoints
  to use the new COVERPOINT macro.
            
 *****************************************************************************/

/*****************************************************************************
 
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/

#include "fc4sc.hpp"

SC_MODULE(stimulus) {

  sc_out<bool> reset;
  sc_out<bool> input_valid;   
  sc_out<int>  sample;  	    
  sc_in<bool>  CLK;

  sc_int<8>     send_value1;
  unsigned cycle;

  class stimulus_coverage : public covergroup {

  public:

    int value = 0;
    int reset = 0;
    int valid = 0;

    // Must call parent constructor somewhere register a new cvg
    CG_CONS(stimulus_coverage) {
    }

    void sample(int value, bool reset, bool valid) {

      this->value = value;
      this->reset = reset;
      this->valid = valid;

      covergroup::sample();

    }

    COVERPOINT(int, values_cvp, value) {
        bin<int>("zero", 0),
        bin_array<int>("lows", 3, interval(1,5)),
        bin<int>("max", 255)
    };

    COVERPOINT(int, reset_cvp, reset) {
        bin<int>("active", 1),
        bin<int>("disabled", 0)
    };

    COVERPOINT(int, input_valid_cvp, valid) {
        bin<int>("valid", 1),
        bin<int>("invalid", 0)
    };

    cross<int, int, int> reset_valid_cross = cross<int, int, int> (this, "reset valid",
        &reset_cvp,
        &input_valid_cvp,
        &values_cvp
    );

  };

  stimulus_coverage stimulus_cg;
  stimulus_coverage empty_cg;

  SC_CTOR(stimulus)
  {
      SC_METHOD(entry);
      dont_initialize();
      sensitive << CLK.pos();
      send_value1 = 0;
      cycle       = 0;
      empty_cg.stop();
  }  
  void entry();
};


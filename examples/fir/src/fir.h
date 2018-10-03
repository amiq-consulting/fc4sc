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
 
  fir.h --

  Original Author: Rocco Jonack, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation: Teodor Vasilache and Dragos Dospinescu,
                         AMIQ Consulting s.r.l. (contributors@amiq.com)
                   Date: 2018-Feb-20

  Description of Modification: Included the FC4SC library, created and
  instantiated a covergroup for collecting shift data coverage.

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

SC_MODULE(fir) {
  
  sc_in<bool>  reset;
  sc_in<bool>  input_valid;        
  sc_in<int>   sample;  	    
  sc_out<bool> output_data_ready;
  sc_out<int>  result;
  sc_in_clk    CLK;

  sc_int<9> coefs[16];

  class shift_coverage : public covergroup {

    public:

    unsigned int shifter = 0;
    
    // Must call parent constructor somewhere register a new cvg
    CG_CONS(shift_coverage) { }

    void sample(sc_int<8> shift_data) {
      this->shifter = shift_data;
      covergroup::sample();
    }

    COVERPOINT(unsigned int, shift_cvp, shifter) {
        bin<unsigned int>("all1", fc4sc::interval(0,255)),
        bin<unsigned int>("all2", fc4sc::interval(0,255))
    };

  };

  shift_coverage shift_cg;


  SC_CTOR(fir)
     {      
      SC_CTHREAD(entry, CLK.pos());
	  reset_signal_is(reset,true);
      #include "fir_const.h"
    }
  
  void entry();
};

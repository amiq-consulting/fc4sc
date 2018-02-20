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

    int SAMPLE_POINT(value,values_cvp);
    int SAMPLE_POINT(reset, reset_cvp);
    int SAMPLE_POINT(valid, input_valid_cvp);

    // Must call parent constructor somewhere register a new cvg
    CG_CONS(test_coverage) {

    }

    void sample(int value, bool reset, bool valid) {

      this->value = value;
      this->reset = reset;
      this->valid = valid;

      covergroup::sample();

    }

    coverpoint<int> values_cvp = coverpoint<int> (this,
        bin<int>("zero", 0),
        bin<int>("low1", make_pair(1,5)),
        bin_array<int>("middle_1", 3, make_pair(6,100)),
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

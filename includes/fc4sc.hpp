/*!
 \file fc.hpp
 \brief Library entry point. Must always be include to use FC4SC.

  Includes all the necessary headers and helpers for FC4SC to work:

    - \link fc4sc::covergroup \endlink definition
    - \link fc4sc::coverpoint \endlink definition
    - \link fc4sc::cross \endlink definition
    - \link fc4sc::bin \endlink / \link fc4sc::illegal_bin \endlink definition

 */

#ifndef FC4SC_HPP
#define FC4SC_HPP

#include "fc4sc_intervals.hpp"
#include "fc4sc_options.hpp"
#include "fc4sc_binsof.hpp"
#include "fc4sc_bin.hpp"
#include "fc4sc_coverpoint.hpp"
#include "fc4sc_cross.hpp"
#include "fc4sc_covergroup.hpp"


using fc4sc::interval;
using fc4sc::bin;
using fc4sc::bin_array;
using fc4sc::binsof;
using fc4sc::illegal_bin;
using fc4sc::coverpoint;
using fc4sc::cross;
using fc4sc::covergroup;

#endif /* FC4SC_HPP */

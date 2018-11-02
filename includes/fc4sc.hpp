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

#include <utility> // std::pair

namespace fc4sc {
  template<typename T>
  using interval_t = std::pair<T, T>;
};

#include "fc4sc_base.hpp"
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
using fc4sc::ignore_bin;
using fc4sc::illegal_bin;
using fc4sc::coverpoint;
using fc4sc::cross;
using fc4sc::covergroup;

#endif /* FC4SC_HPP */

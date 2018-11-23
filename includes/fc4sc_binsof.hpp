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

#ifndef FC4SC_BINSONF_HPP
#define FC4SC_BINSONF_HPP

#include "fc4sc_intervals.hpp"

namespace fc4sc {

template <typename T>
class coverpoint;

template <typename T>
class bin;

template <typename T>
class binsof {
  // TODO: implementation needed
public:

  std::vector<interval_t<T>> allowed_bins;

  binsof() {}

  binsof(coverpoint<T> cvp) {
    (void)cvp; // suppress warning
  };

  binsof(bin<T> cvp) {
    (void)cvp; // suppress warning
  };

  binsof<T>& operator||(const binsof<T> &rhs) {
    (void)rhs; // suppress warning
    return *this;
  };

  binsof<T>& operator&&(const binsof<T> &rhs) {
    (void)rhs; // suppress warning
    return *this;
  };

  binsof<T>& intersect() {
    return *this;
  }
};

}	// namespace fc4sc

#endif


#pragma once

#include "fc4sc_intervals.hpp"

namespace fc4sc {

template <typename T>
class coverpoint;

template <typename T>
class bin;

template <typename T>
class binsof {

public:

	vector<pair<T,T>> allowed_bins;

	binsof() {};
	
	binsof(coverpoint<T> cvp) {

	};

	binsof(bin<T> cvp) {
		
	};

	binsof<T>& operator||(const binsof<T> &rhs) {

	};

	binsof<T>& operator&&(const binsof<T> &rhs) {

	};

	binsof<T>& intersect() {

	}

};

}	// namespace fc4sc
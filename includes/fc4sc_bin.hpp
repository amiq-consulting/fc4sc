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
 \file bin.hpp
 \brief Template functions for bin implementations

 This file contains the template implementation of
 bins and illegal bins
 */

#ifndef FC4SC_BIN_HPP
#define FC4SC_BIN_HPP

#include <iostream>
#include <algorithm>
#include <tuple>
#include <vector>
#include <stack>
#include <memory>  // unique_ptr

#include "fc4sc_base.hpp"

using std::to_string;

using std::cerr;
using std::cout;

using std::string;

using std::lower_bound;
using std::sort;
using std::vector;

using std::tuple;
using std::tuple_cat;

namespace fc4sc
{

template <typename T>
class coverpoint;

template <typename T>
class binsof;

template <typename T>
class bin;

template <typename T>
static vector<interval_t<T>> reunion(const bin<T>& lhs, const bin<T>& rhs);

template <typename T>
static vector<interval_t<T>> reunion(const bin<T>& lhs, const vector<interval_t<T>>& rhs);

template <typename T>
static vector<interval_t<T>> intersection(const bin<T>& lhs, const bin<T>& rhs);

template <typename T>
static vector<interval_t<T>> intersection(const bin<T>& lhs, const vector<interval_t<T>>& rhs);

/*!
 * \brief Defines a class for default bins
 * \tparam T Type of values in this bin
 */
template <class T>
class bin : public bin_base
{
  static_assert(std::is_arithmetic<T>::value, "Type must be numeric!");
    
  friend binsof<T>;
  friend coverpoint<T>;

protected:
  /* Virtual function used to register this bin inside a coverpoint */
  virtual void add_to_cvp(coverpoint<T> &cvp) const
  {
    cvp.bins.push_back(*this);
  }

  void print_xml_header(std::ostream &stream, const std::string &type) const
  {
    stream << "<ucis:coverpointBin name=\"" << name << "\" \n";
    stream << "type=\""
           << type
           << "\" "
           << "alias=\"" << get_hitcount() << "\""
           << ">\n";
  }

  uint64_t hits = 0;

  /*! Storage for the values. All are converted to intervals */
  vector<interval_t<T>> intervals;

  /*! Name of the bin */
  string name;

public:

  /*!
   *  \brief Takes a value and wraps it in a pair
   *  \param value Value associated with the bin
   *  \param args Rest of arguments
   */
  template <typename... Args>
  bin(T value, Args... args) : bin(args...)
  {
    intervals.push_back(interval(value, value));
  }

  /*!
   *  \brief Adds a new interval to the bin
   *  \param interval New interval associated with the bin
   *  \param args Rest of arguments
   */
  template <typename... Args>
  bin(interval_t<T> interval, Args... args) : bin(args...)
  {
    if (interval.first > interval.second)
    {
      std::swap(interval.first, interval.second);
    }

    intervals.insert(intervals.begin(), interval);
  }

  /*!
   *  \brief Takes the bin name
   *  \param bin_name Name of the bin
   *  \param args Rest of arguments
   */
  template <typename... Args>
  bin(const string &bin_name, Args... args) : bin(args...)
  {
    this->name = bin_name;
  }

  /*!
   *  \brief Default constructor
   */
  bin() {}

  /*! Destructor */
  virtual ~bin(){};

  uint64_t get_hitcount() const
  {
    return hits;
  }

  /*!
   * \brief Samples the given value and increments hit counts
   * \param val Current sampled value
   */
  uint64_t sample(const T &val)
  {

    // Just search for the value in the intervals we have
    for (auto i : intervals)
      if (val >= i.first && val <= i.second)
      {
        this->hits++;
        return 1;
      }

    return 0;
  }

  /*!
   * \brief Finds if the given value is contained in the bin
   * \param val Value to search for
   * \returns true if found, false otherwise
   */
  bool contains(const T &val) const
  {

    for (uint i = 0; i < intervals.size(); ++i)
      if (intervals[i].first <= val && intervals[i].second >= val)
        return true;

    return false;
  }

  bool is_empty() const
  {
    return intervals.empty();
  }

  /*!
   * \brief Writes the bin in UCIS XML format
   * \param stream Where to print it
   */
  virtual void to_xml(std::ostream &stream) const
  {

    print_xml_header(stream, "default");

    // Print each range. Coverpoint writes the header (name etc.)
    for (uint i = 0; i < intervals.size(); ++i)
    {
      stream << "<ucis:range \n"
             << "from=\"" << intervals[i].first << "\" \n"
             << "to =\"" << intervals[i].second << "\"\n"
             << ">\n";

      // Print hits for each range
      stream << "<ucis:contents "
             << "coverageCount=\"" << this->hits << "\""
             << ">";
      stream << "</ucis:contents>\n";

      stream << "</ucis:range>\n\n";
    }

    stream << "</ucis:coverpointBin>\n";
  }

  friend vector<interval_t<T>> reunion<T>(const bin<T>& lhs, const vector<interval_t<T>>& rhs);
  friend vector<interval_t<T>> intersection<T>(const bin<T>& lhs, const vector<interval_t<T>>& rhs);

  friend vector<interval_t<T>> reunion<T>(const bin<T>& lhs, const bin<T>& rhs);
  friend vector<interval_t<T>> intersection<T>(const bin<T>& lhs, const bin<T>& rhs);

};

/*!
 * \brief Defines a class for illegal bins
 * \tparam T Type of values in this bin
 */
template <class T>
class illegal_bin final : public bin<T>
{
  static_assert(std::is_arithmetic<T>::value, "Type must be numeric!");
protected:
  /* Virtual function used to register this bin inside a coverpoint */
  virtual void add_to_cvp(coverpoint<T> &cvp) const override
  {
    cvp.illegal_bins.push_back(*this);
  }

public:
  /*!
   *  \brief Forward to parent constructor
   */
  template <typename... Args>
  explicit illegal_bin(Args... args) : bin<T>::bin(args...){};

  virtual ~illegal_bin(){};

  /*!
   * \brief Same as bin::sample(const T& val)
   *
   * Samples the inner bin instance, but throws an error if the value is found
   */
  uint64_t sample(const T &val)
  {

    for (uint i = 0; i < this->intervals.size(); ++i)
      if (val >= this->intervals[i].first && val <= this->intervals[i].second)
      {
        this->hits++;
        throw string("illegal sample");
        return 1;
      }

    return 0;
  }

  virtual void to_xml(std::ostream &stream) const
  {

    this->print_xml_header(stream, "illegal");

    // Print each range. Coverpoint writes the header (name etc.)
    for (uint i = 0; i < this->intervals.size(); ++i)
    {
      stream << "<ucis:range \n"
             << "from=\"" << this->intervals[i].first << "\" \n"
             << "to =\"" << this->intervals[i].second << "\"\n"
             << ">\n";

      // Print hits for each range
      stream << "<ucis:contents "
             << "coverageCount=\"" << this->hits << "\""
             << ">";
      stream << "</ucis:contents>\n";

      stream << "</ucis:range>\n\n";
    }

    stream << "</ucis:coverpointBin>\n";
  }
};

template <class T>
class ignore_bin final : public bin<T>
{
  static_assert(std::is_arithmetic<T>::value, "Type must be numeric!");
protected:
  /* Virtual function used to register this bin inside a coverpoint */
  virtual void add_to_cvp(coverpoint<T> &cvp) const override
  {
    cvp.ignore_bins.push_back(*this);
  }
public:
  /*!
   *  \brief Forward to parent constructor
   */
  template <typename... Args>
  explicit ignore_bin(Args... args) : bin<T>::bin(args...){};

  virtual ~ignore_bin(){};
};


template <class T>
class bin_array final : public bin<T>
{
  static_assert(std::is_arithmetic<T>::value, "Type must be numeric!");
protected:
  /* Virtual function used to register this bin inside a coverpoint */
  virtual void add_to_cvp(coverpoint<T> &cvp) const override
  {
    cvp.bin_arrays.push_back(*this);
  }
public:
  vector<uint64_t> split_hits;
  uint64_t count;

  explicit bin_array(const string &name, int count, interval_t<T> interval) : bin<T>(name, interval), count(count)
  {

    auto intv = this->intervals[0];
    uint64_t interval_length = (intv.second - intv.first) + 1;

    if (this->count > interval_length)
      this->count = 1;

    split_hits.resize(this->count);
  }

  virtual ~bin_array() {}

  uint64_t size() {
    return this->count;
  }

  /*!
   * \brief Same as bin::sample(const T& val)
   *
   */
  uint64_t sample(const T &val)
  {
    for (auto &interval : this->intervals)
      if (val >= interval.first && val <= interval.second)
      {
        uint64_t bin_index = (val - interval.first) * count / (interval.second - interval.first);
        if (bin_index == count)
          bin_index--;

        this->split_hits[bin_index]++;
        return bin_index + 1;
      }

    return 0;
  }



  virtual void print_range(std::ostream &stream, T start, T stop, T step, uint index) const
  {

    if (index != 0)
      start++;

     if (index == (count - 1)) {
      start = stop - step + 1;
      step--;
     }

    stream << "<ucis:range \n"
           << "from=\"" << start << "\" \n";

    stream << "to =\"" << (start + step) << "\"\n"
           << ">\n";
  }

  virtual void to_xml(std::ostream &stream) const
  {
    T start = this->intervals[0].first;
    T stop = this->intervals[0].second;

    T step = (stop - start + 1) / count;

    for (uint i = 0; i < count; ++i)
    {
      stream << "<ucis:coverpointBin name=\"" << this->name << "_" << i+1 << "\" \n";
      stream << "type=\"" << "default" << "\" "
             << "alias=\"" << this->split_hits[i] << "\"" << ">\n";

      print_range(stream, start, stop, step, i);
          // Print hits for each range
          stream
          << "<ucis:contents "
          << "coverageCount=\"" << this->split_hits[i] << "\""
          << ">";
      stream << "</ucis:contents>\n";

      stream << "</ucis:range>\n\n";

      stream << "</ucis:coverpointBin>\n";

      start = start + step;
    }
  }
};

/*
 * Bin wrapper class used when constructing coverpoint via the COVERPOINT macro.
 * Under the hood, the macro instantiates a coverpoint using std::initializer_list
 * as argument. Because the std::initializer_list is limited to one type only,
 * we cannot directly pass any type of bin we want to the coverpoint. In order to
 * do that, we use this class which offers implicit cast from any type of bin and
 * stores it internally as a dynamically allocated object.
 */
template <class T>
class bin_wrapper final {
private:
  friend class coverpoint<T>;

  std::unique_ptr<bin<T>> bin_h;
  bin<T> *get_bin() const { return bin_h.get(); }


public:
  // Implicit cast to other bin types.
  bin_wrapper(const bin<T>& r) : bin_h(new bin<T>(r)) {}
  bin_wrapper(const bin_array<T>& r) : bin_h(new bin_array<T>(r)) {}
  bin_wrapper(const illegal_bin<T>& r) : bin_h(new illegal_bin<T>(r)) {}
  bin_wrapper(const ignore_bin<T>& r) : bin_h(new ignore_bin<T>(r)) {}

  ~bin_wrapper() = default;

  bin_wrapper() = delete;
  bin_wrapper(bin_wrapper &) = delete;
  bin_wrapper(bin_wrapper &&) = delete;
  bin_wrapper& operator=(bin_wrapper &) = delete;
  bin_wrapper& operator=(bin_wrapper &&) = delete;
};

} // namespace fc4sc

#endif /* FC4SC_BIN_HPP */

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
#include <vector>
#include <sstream>
#include <memory>  // unique_ptr

#include "fc4sc_base.hpp"

namespace fc4sc
{
// forward declarations
template <typename T> class coverpoint;
template <typename T> class binsof;
template <typename T> class bin;

template <typename T>
static std::vector<interval_t<T>> reunion(const bin<T>& lhs, const bin<T>& rhs);

template <typename T>
static std::vector<interval_t<T>> reunion(const bin<T>& lhs, const std::vector<interval_t<T>>& rhs);

template <typename T>
static std::vector<interval_t<T>> intersection(const bin<T>& lhs, const bin<T>& rhs);

template <typename T>
static std::vector<interval_t<T>> intersection(const bin<T>& lhs, const std::vector<interval_t<T>>& rhs);

/*!
 * \brief Defines a class for default bins
 * \tparam T Type of values in this bin
 */
template <class T>
class bin : public bin_base
{
private:
  static_assert(std::is_arithmetic<T>::value, "Type must be numeric!");
  friend binsof<T>;
  friend coverpoint<T>;

  // These constructors are private, making the only public constructor be
  // the one which receives a name as the first argument. This forces the user
  // to give a name for each instantiated bin.

  /*!
   *  \brief Takes a value and wraps it in a pair
   *  \param value Value associated with the bin
   *  \param args Rest of arguments
   */
  template <typename... Args>
  bin(T value, Args... args) noexcept : bin(args...) {
    intervals.push_back(interval(value, value));
  }

  /*!
   *  \brief Adds a new interval to the bin
   *  \param interval New interval associated with the bin
   *  \param args Rest of arguments
   */
  template <typename... Args>
  bin(interval_t<T> interval, Args... args) noexcept : bin(args...) {
    intervals.push_back(interval);
    if (intervals.back().first > intervals.back().second) {
      std::swap(intervals.back().first, intervals.back().second);
    }
  }

protected:
  /*! Default Constructor */
  bin() = default;

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
  std::vector<interval_t<T>> intervals;

  /*! Name of the bin */
  std::string name;

public:
  /* Virtual function used to register this bin inside a coverpoint */
  virtual void add_to_cvp(coverpoint<T> &cvp) const
  {
    cvp.bins.push_back(*this);
  }

  /*!
   *  \brief Takes the bin name
   *  \param bin_name Name of the bin
   *  \param args Rest of arguments
   */
  template<typename ...Args>
  bin(const std::string &bin_name, Args... args) noexcept : bin(args...) {
    static_assert(forbid_type<std::string, Args...>::value, "Bin constructor accepts only 1 name argument!");
    this->name = bin_name;
  }

  /*! Default Destructor */
  virtual ~bin() = default;

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
    for (size_t i = 0; i < intervals.size(); ++i)
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
    for (size_t i = 0; i < intervals.size(); ++i)
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

  friend std::vector<interval_t<T>> reunion<T>(const bin<T>& lhs, const std::vector<interval_t<T>>& rhs);
  friend std::vector<interval_t<T>> intersection<T>(const bin<T>& lhs, const std::vector<interval_t<T>>& rhs);

  friend std::vector<interval_t<T>> reunion<T>(const bin<T>& lhs, const bin<T>& rhs);
  friend std::vector<interval_t<T>> intersection<T>(const bin<T>& lhs, const bin<T>& rhs);

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

  illegal_bin() = delete;
public:
  /*!
   *  \brief Forward to parent constructor
   */
  template <typename... Args>
  explicit illegal_bin(Args... args) : bin<T>::bin(args...){}

  virtual ~illegal_bin() = default;

  /*!
   * \brief Same as bin::sample(const T& val)
   *
   * Samples the inner bin instance, but throws an error if the value is found
   */
  uint64_t sample(const T &val)
  {
    for (size_t i = 0; i < this->intervals.size(); ++i)
      if (val >= this->intervals[i].first && val <= this->intervals[i].second) {
        // construct exception to be thrown
        std::stringstream ss; ss << val;
        illegal_bin_sample_exception e;
        e.update_bin_info(this->name, ss.str());
        this->hits++;
        throw e;
      }

    return 0;
  }

  virtual void to_xml(std::ostream &stream) const
  {

    this->print_xml_header(stream, "illegal");

    // Print each range. Coverpoint writes the header (name etc.)
    for (size_t i = 0; i < this->intervals.size(); ++i)
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

  ignore_bin() = delete;
public:
  /*!
   *  \brief Forward to parent constructor
   */
  template <typename... Args>
  explicit ignore_bin(Args... args) : bin<T>::bin(args...){}

  virtual ~ignore_bin() = default;
};


template <class T>
class bin_array final : public bin<T>
{
  static_assert(std::is_arithmetic<T>::value, "Type must be numeric!");
protected:
  /* Virtual function used to register this bin inside a coverpoint */
  virtual void add_to_cvp(coverpoint<T> &cvp) const override
  {
    if (this->sparse) {
      // bin array was defined by using a vector of intervals or values
      // create a new bin for each value/interval and add it to the coverpoint
      std::stringstream ss;
      for (int i = 0; i < this->intervals.size(); ++i) {
        ss << this->name << "_" << i;
        cvp.bins.push_back(bin<T>(ss.str(), this->intervals[i]));
        ss.str(std::string()); // clear the stringstream
      }
    }
    else {
      // bin array was defined by using an interval which needs to be split into
      // multiple pieces. The interval is found in the this->intervals[0]
      T interval_length = (this->intervals[0].second - this->intervals[0].first) + 1;
      if (this->count > interval_length) {
        // This bin array interval cannot be split into pieces. Add a single
        // bin containing the whole interval to the coverpoint. We can simply
        // use this object since it already matches the bin that we need!
        cvp.bins.push_back(*this);
      }
      else {
        std::stringstream ss;
        // This bin array interval must be split into pieces.
        T start = this->intervals[0].first;
        T stop = this->intervals[0].second;
        T interval_len = (interval_length + 1) / this->count;

        for (size_t i = 0; i < this->count; ++i) {
          ss << this->name << "_" << i;
          // the last interval, will contain all the extra elements
          T end = (i == (this->count - 1)) ? stop : start + (interval_len - 1);
          cvp.bins.push_back(bin<T>(ss.str(), interval(start, end)));
          start = start + interval_len;
          ss.str(std::string()); // clear the stringstream
        }
      }
    }
  }

  uint64_t count;
  bool sparse = false;

  bin_array() = delete;

public:
  /*!
   * \brief Constructs an bin_array which will split an interval into multiple
   * equal parts. The number of sub-intervals is specified via the count argument
   */
  explicit bin_array(const std::string &name, uint64_t count, interval_t<T> interval) noexcept :
    bin<T>(name, interval), count(count), sparse(false) {}

  /*!
   * \brief Constructs an bin_array from a vector of intervals where each
   * interval will be nested by a bin
   */
  explicit bin_array(const std::string &name, std::vector<interval_t<T>>&& intvs) noexcept :
    count(intvs.size()), sparse(true)
  {
    this->name = name;
    this->intervals = std::move(intvs);
  }

  /*!
   * \brief Constructs an bin_array from a vector of values where each
   * values will be nested by a bin
   */
  explicit bin_array(const std::string &name, const std::vector<T>& intvs) noexcept :
    count(intvs.size()), sparse(true)
  {
    this->name = name;
    this->intervals.clear();
    this->intervals.reserve(this->count);
    // transform each value in the input vector to an interval
    std::transform(intvs.begin(), intvs.end(),
                   std::back_inserter(this->intervals),
                   [](const T& v) { return fc4sc::interval(v,v); });
  }

  virtual ~bin_array() = default;

//  uint64_t size() {
//    return this->count;
//  }

//  /*!
//   * \brief Same as bin::sample(const T& val)
//   *
//   */
//  uint64_t sample(const T &val)
//  {
//    uint64_t bin_index = 0;
//    for (auto &interval : this->intervals) {
//      if (val >= interval.first && val <= interval.second)
//      {
//        if (!sparse) {
//          bin_index = (val - interval.first) * count / (interval.second - interval.first);
//          if (bin_index == count)
//            bin_index--;
//        }
//        this->split_hits[bin_index]++;
//        return bin_index + 1;
//      }
//      bin_index++;
//    }
//    return 0;
//  }
//
//  virtual void to_xml(std::ostream &stream) const
//  {
//    T start = this->intervals[0].first;
//    T stop = this->intervals[0].second;
//    T step = (stop - start + 1) / count;
//
//    for (size_t i = 0; i < count; ++i)
//    {
//      stream << "<ucis:coverpointBin name=\"" << this->name << "_" << i+1 << "\" \n";
//      stream << "type=\"" << "default" << "\" "
//             << "alias=\"" << this->split_hits[i] << "\"" << ">\n";
//
//      if (!sparse) {
//        T end = (i == (count - 1)) ? stop : start + step;
//        stream << "<ucis:range \n"
//               << "from=\"" << start << "\" \n";
//        stream << "to =\"" << end << "\"\n"
//               << ">\n";
//        start = start + step;
//      }
//      else {
//        stream << "<ucis:range \n"
//               << "from=\"" << this->intervals[i].first << "\" \n";
//        stream << "to =\"" << this->intervals[i].second << "\"\n"
//               << ">\n";
//      }
//      stream // Print hits for each range
//        << "<ucis:contents "
//        << "coverageCount=\"" << this->split_hits[i] << "\""
//        << ">";
//      stream << "</ucis:contents>\n";
//      stream << "</ucis:range>\n\n";
//      stream << "</ucis:coverpointBin>\n";
//    }
//  }
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

  bin_wrapper() = delete;
  bin_wrapper(bin_wrapper &) = delete;
  bin_wrapper(bin_wrapper &&) = delete;
  bin_wrapper& operator=(bin_wrapper &) = delete;
  bin_wrapper& operator=(bin_wrapper &&) = delete;
public:
  ~bin_wrapper() = default;
  // Implicit cast to other bin types.
  bin_wrapper(bin<T>        && r) noexcept : bin_h(new bin<T>        (std::move(r))) {}
  bin_wrapper(bin_array<T>  && r) noexcept : bin_h(new bin_array<T>  (std::move(r))) {}
  bin_wrapper(illegal_bin<T>&& r) noexcept : bin_h(new illegal_bin<T>(std::move(r))) {}
  bin_wrapper(ignore_bin<T> && r) noexcept : bin_h(new ignore_bin<T> (std::move(r))) {}
};

} // namespace fc4sc

#endif /* FC4SC_BIN_HPP */

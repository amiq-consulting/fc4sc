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

#include "fc4sc_base.hpp"

using std::to_string;

using std::cerr;
using std::cout;

using std::pair;
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
static vector<pair<T,T>> reunion(const bin<T>& lhs, const bin<T>& rhs);

template <typename T>
static vector<pair<T,T>> reunion(const bin<T>& lhs, const vector<pair<T,T>>& rhs);

template <typename T>
static vector<pair<T,T>> intersection(const bin<T>& lhs, const bin<T>& rhs);

template <typename T>
static vector<pair<T,T>> intersection(const bin<T>& lhs, const vector<pair<T,T>>& rhs);


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

  void print_xml_header(ostream &stream, const string &type) const
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
  vector<pair<T, T>> intervals;

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
  bin(pair<T, T> interval, Args... args) : bin(args...)
  {

    if (interval.first > interval.second)
    {
      std::swap(interval.first, interval.second);
    }

    intervals.push_back(interval);
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
  virtual void to_xml(ostream &stream) const
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

  friend vector<pair<T,T>> reunion<T>(const bin<T>& lhs, const vector<pair<T,T>>& rhs);
  friend vector<pair<T,T>> intersection<T>(const bin<T>& lhs, const vector<pair<T,T>>& rhs);

  friend vector<pair<T,T>> reunion<T>(const bin<T>& lhs, const bin<T>& rhs);
  friend vector<pair<T,T>> intersection<T>(const bin<T>& lhs, const bin<T>& rhs);

};

/*!
 * \brief Defines a class for illegal bins
 * \tparam T Type of values in this bin
 */
template <class T>
class illegal_bin : public bin<T>
{

  static_assert(std::is_arithmetic<T>::value, "Type must be numeric!");

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

  virtual void to_xml(ostream &stream) const
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
class ignore_bin : public bin<T>
{
  static_assert(std::is_arithmetic<T>::value, "Type must be numeric!");

public:
  /*!
   *  \brief Forward to parent constructor
   */
  template <typename... Args>
  explicit ignore_bin(Args... args) : bin<T>::bin(args...){};

  virtual ~ignore_bin(){};
};


template <class T>
class bin_array : public bin<T>
{
  static_assert(std::is_arithmetic<T>::value, "Type must be numeric!");

public:
  vector<uint64_t> split_hits;
  uint64_t count;

  explicit bin_array(const string &name, int count, pair<T, T> interval) : bin<T>(name, interval), count(count)
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
        int bin_index = (val - interval.first) * count / (interval.second - interval.first);
        if (bin_index == count)
          bin_index--;

        this->split_hits[bin_index]++;
        return bin_index + 1;
      }

    return 0;
  }



  virtual void print_range(ostream &stream, T start, T stop, T step, uint index) const
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

  virtual void to_xml(ostream &stream) const
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

} // namespace fc4sc

#endif /* FC4SC_BIN_HPP */

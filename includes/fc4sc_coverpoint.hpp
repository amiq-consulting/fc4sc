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
 \file coverpoint.hpp
 \brief Helper functions and interfaces

 This file contains helper functions and the base classes
 for bins, coverpoints and covergroups.
 */

#ifndef FC4SC_COVERPOINT_HPP
#define FC4SC_COVERPOINT_HPP

#include <type_traits>
#include <list>
#include <unordered_map>
#include <functional>
#include <tuple>
#include <algorithm> // std::find

#include "fc4sc_bin.hpp"

using std::tuple;
using std::get;

namespace fc4sc
{

// Friend classes forward declaration
template <typename T>
class binsof;

template <typename ... Args>
class cross;

/*!
 * \brief Defines a class for coverpoints
 * \tparam T Type of bins that this coverpoint is holding
 */

template <class T>
class coverpoint;

class covergroup;

template <class T>
class coverpoint final : public cvp_base
{
private:
  // static check to make sure that the coverpoint is templated by a numeric type
  static_assert(std::is_arithmetic<T>::value, "Type must be numeric!");
  /*
   * The covergroup class needs access to private members of the coverpoint in order to
   * set the sample condition, expression and name.
   * This id done in function
   */
  friend class covergroup;
  // friend functions that can insert into the bin vectors -> add_to_cvp(coverpoint<T>&)
  friend class bin<T>;
  friend class bin_array<T>;
  friend class ignore_bin<T>;
  friend class illegal_bin<T>;

  bool has_sample_expression = false;
  /*!
   * String-ified sample expression. It can be used for reporting.
   * When the string is empty, no sample expression is registered.
   */
  std::string sample_expression_str;
  /*! Expression used to sample the data */
  std::function<T()> sample_expression;

  /*!
   * String-ified sample condition. It can be used for reporting.
   * When the string is empty, no sample condition is registered.
   */
  std::string sample_condition_str;
  /*! Condition based on which the sampling takes place or not */
  std::function<bool()> sample_condition;

  // pointer sample variable (assigned via SAMPLE_POINT macro)
  T* sample_point = nullptr;

  /*! bins contained in this coverpoint */
  vector<bin<T>> bins;
  /*! bin_arrays contained in this coverpoint */
  vector<bin_array<T>> bin_arrays;
  /*! Illegal bins contained in this coverpoint */
  vector<illegal_bin<T>> illegal_bins;
  /*! Ignore bins contained in this coverpoint */
  vector<ignore_bin<T>> ignore_bins;

  /*! Sampling switch */
  bool colect = true;

  bool has_sample_condition() { return !sample_condition_str.empty(); }

  //
  coverpoint<T>& operator=(coverpoint<T>& rh) = delete;
  /*
   * kept for backwards compatibility with previous instantiation method:
   * coverpoint<int> cvp = coverpoint<int> (...);
   */
  // coverpoint(coverpoint<T>&& rh) = delete;

  /*!
   *  \brief Sampling function at coverpoint level
   *  \param cvp_val Value to be sampled for this coverpoint
   *
   *  Takes a value and searches it in the owned bins
   */
  void sample(const T &cvp_val)  {

#ifdef FC4SC_DISABLE_SAMPLING
    return;
#endif

    if (!colect) return;

    for (auto& ig_bin_it : ignore_bins)
      if(ig_bin_it.sample(cvp_val)) {
        this->last_sample_success = 0;
        return;
      }

    // First search in illegal bins
    for (auto& il_bin_hit : illegal_bins) {
      try { il_bin_hit.sample(cvp_val);  }
      catch (const string &e)
      {
        // Illegal bin hit -> show error
        cerr << "Illegal sample in [" << p_name << "/" << name << "/" << il_bin_hit.name << "] on value [" << cvp_val << "]!\n";
        this->last_sample_success = 0;
#ifndef FC4SC_NO_THROW // By default the simulation will stop
        cerr << "Stopping simulation\n";
        throw(e);
#endif
      }
    }

    // Sample bin arrays first => higher hit chance
    uint64_t start = bins.size();
    for (uint i = 0; i < bin_arrays.size(); ++i) {

      uint64_t hit = bin_arrays[i].sample(cvp_val);
      if (hit) {
        this->last_bin_index_hit = start + hit - 1;
        this->last_sample_success = 1;
        return;
      }

      start += bin_arrays[i].count;
    }

    // Sample default bins
    for (uint i = 0; i < bins.size(); ++i) {
      if (bins[i].sample(cvp_val)) {
        this->last_bin_index_hit = i;
        this->last_sample_success = 1;
        return;
      }
    }
  
    this->last_sample_success = 0;
    misses++;
  }

public:
  /*
   * Initialization constructor. It is used as a "hack" designed to extract the
   * sampling expression and condition from the enclosing covergroup.
   */
  coverpoint(const coverpoint<T>& rh) {
    // TODO: add mechanism to make sure that this is only called by the use of COVERGROUP macro;
    this->has_sample_expression = true;
    this->sample_expression = rh.sample_expression;
    this->sample_condition = rh.sample_condition;
    this->sample_expression_str = rh.sample_expression_str;
    this->sample_condition_str = rh.sample_condition_str;
    this->bins = rh.bins;
    this->bin_arrays = rh.bin_arrays;
    this->ignore_bins = rh.ignore_bins;
    this->illegal_bins = rh.illegal_bins;
    this->name = rh.name;
  }

  coverpoint<T>& operator=(coverpoint<T>&& rh) {
    // TODO: add mechanism to make sure that this is only called by the use of COVERGROUP macro;
    this->bins = std::move(rh.bins);
    this->bin_arrays = std::move(rh.bin_arrays);
    this->ignore_bins = std::move(rh.ignore_bins);
    this->illegal_bins = std::move(rh.illegal_bins);
    return *this;
  }

  /*! Default constructor */
  coverpoint()
  {
    name = "";
    colect = true;
    misses = 0;
  }

  ~coverpoint() = default;

  /*!
   * \brief Initializer list constructor that receives a list of bin (of any types,
   * even mixed) and registers all the bins in this coverpoint.
   */
  coverpoint(std::initializer_list<bin_wrapper<T>> list) {
    for (auto &bin_w : list) {
      bin_w.get_bin()->add_to_cvp(*this);
    }
  }

  // TODO: remove all constructors after completing the implementation of the sample expression?
  /*!
   *  \brief Constructor that registers a new default bin
   */
  template <typename... Args>
  coverpoint(bin<T> n, Args... args) : coverpoint(args...)
  {
    if (!n.is_empty())
    {
      reverse(n.intervals.begin(), n.intervals.end());
      bins.push_back(n);
    }
  }

  /*!
   *  \brief Constructor that registers a new illegal bin
   */
  template < typename... Args>
  coverpoint(bin_array<T> n, Args... args) : coverpoint(args...)
  {
    bin_arrays.push_back(n);
  }

  /*!
   *  \brief Constructor that registers a new illegal bin
   */
  template <typename... Args>
  coverpoint(illegal_bin<T> n, Args... args) : coverpoint(args...)
  {

    if (!n.is_empty())
    {
      reverse(n.intervals.begin(), n.intervals.end());
      illegal_bins.push_back(n);
    }
  }

  /*!
   *  \brief Constructor that registers a new ignore bin
   */
  template <typename... Args>
  coverpoint(ignore_bin<T> n, Args... args) : coverpoint(args...)
  {
    if (!n.is_empty())
    {
      reverse(n.intervals.begin(), n.intervals.end());
      ignore_bins.push_back(n);
    }
  }

  /*!
   *  \brief Constructor that takes the parent covergroup.
   */
  template <typename... Args>
  coverpoint(cvg_base *n, Args... args) : coverpoint(args...)
  {
    reverse(bins.begin(), bins.end());
   
    n->cvps.push_back(this);
    p_name = n->name;

    // set strings here
    auto strings = n->get_strings(this);

    this->sample_point = static_cast<T *>(get<0>(strings));
    this->name = get<1>(strings);
    this->sample_expression_str = get<2>(strings);
  }

  template <typename... Args>
  coverpoint(const string& cvp_name , Args... args)   : coverpoint(args...)
  {
    this->name = cvp_name;
  }

  /*!
   *  Retrieves the number of default bins
   */
  uint64_t size() {
    uint64_t total_size = 0;
    for (auto arr : bin_arrays)
      total_size += arr.size();
    return total_size + bins.size();
  }

  void sample() 
  {
    if (has_sample_expression) {
      if (!has_sample_condition() || sample_condition())
         this->sample(sample_expression());
    }
    else {
      this->sample(*sample_point);
    }
  }

  /*!
   *  \brief Computes coverage for this instance
   *  \returns Coverage value as a double between 0 and 100
   */
  double get_inst_coverage() const 
  {
    double res = 0;

    if (bins.empty() && bin_arrays.empty()) // no bins or bin arrays defined
    {
      if (option.weight == 0)
        return 100;
      else
        return 0;
    }

    for (auto &bin : bins)
      res += (bin.get_hitcount() >= option.at_least);

    uint64_t extra_bins = 0;
    for (auto &bin_array : bin_arrays) {
      vector<uint64_t> hits = bin_array.split_hits;

      for (uint64_t hitcount : hits) 
        res += (hitcount >= option.at_least);

      extra_bins += bin_array.count;
    }

    double real = res * 100 / (bins.size() + extra_bins);

    return (real >= this->option.goal) ? 100 : real;
  }

  /*!
   *  \brief Computes coverage for this instance
   *  \param covered Number of covered bins
   *  \param total Total number of bins in this coverpoint
   *  \returns Coverage value as a double between 0 and 100
   */
  double get_inst_coverage(int &covered, int &total) const 
  {

    double res = 0;

    covered = 0;
    total = bins.size();

    if (!bins.size() && !bin_arrays.size())
    {

      total = 0;

      if (option.weight == 0)
        return 100;
      else
        return 0;
    }

    for (auto &bin : bins)
      res += (bin.get_hitcount() >= option.at_least);

    uint64_t extra_bins = 0;
    for (auto &bin_array : bin_arrays) {
      vector<uint64_t> hits = bin_array.split_hits;

      for (uint64_t hitcount : hits) 
        res += (hitcount >= option.at_least);

      extra_bins += bin_array.count;
    }

    total += extra_bins;
    covered = res;

    double real = res * 100 / total;

    return (real >= this->option.goal) ? 100 : real;
  }

  /*!
   *  \brief Changes the instances name
   *  \param new_name New associated name
   */
  void set_inst_name(const string &new_name)
  {
    this->name = new_name;
  }

  /*!
   * \brief Enables sampling
   */
  void start()
  {
    colect = true;
  }

  /*!
   * \brief Enables sampling
   */
  void stop()
  {
    colect = false;
  }

  /*!
   * \brief print instance in UCIS XML format
   * \param stream Where to print
   */
  virtual void to_xml(ostream &stream) const
  {
    stream << "<ucis:coverpoint ";
    stream << "name=\"" << this->name << "\" ";
    stream << "key=\""
           << "KEY"
           << "\" ";
    stream << "exprString=\"" << this->sample_expression_str << "\"";
    stream << ">\n";

    stream << option << "\n";
  
    for (auto &bin : bins)
      bin.to_xml(stream);

    for (auto& bin_arr : bin_arrays)
      bin_arr.to_xml(stream); 

    for (auto &bin : illegal_bins)
      bin.to_xml(stream);

    stream << "</ucis:coverpoint>\n\n";
  }

};

} // namespace fc4sc

#endif /* FC4SC_COVERPOINT_HPP */

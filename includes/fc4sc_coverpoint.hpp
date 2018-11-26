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
   * The covergroup class needs access to private members of the coverpoint
   * in order to set the sample condition, expression and name.
   */
  friend class covergroup;
  // friend functions that need access to the bin vectors -> add_to_cvp(coverpoint<T>&)
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
  std::vector<bin<T>> bins;
  /*! Illegal bins contained in this coverpoint */
  std::vector<illegal_bin<T>> illegal_bins;
  /*! Ignore bins contained in this coverpoint */
  std::vector<ignore_bin<T>> ignore_bins;

  /*! Sampling switch */
  bool collect = true;

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
    if (!collect) return;
    this->last_sample_success = false;

    // 1) Search if the value is in the ignore bins
    for (auto& ig_bin_it : ignore_bins) {
      if (ig_bin_it.sample(cvp_val)) {
        misses++;
        return;
      }
    }
    // 2) Search if the value is in the illegal bins
    for (auto& il_bin_hit : illegal_bins) {
      try { il_bin_hit.sample(cvp_val); }
      catch (illegal_bin_sample_exception &e) {
        e.update_cvp_info(this->name);
        throw e;
      }
    }
    // Sample regular bins
    for (size_t i = 0; i < bins.size(); ++i) {
      if (bins[i].sample(cvp_val)) {
        this->last_bin_index_hit = i;
        this->last_sample_success = true;
        if (this->stop_sample_on_first_bin_hit) return;
      }
    }
  
    if (!this->last_sample_success) { misses++; }
  }

  /*! Default constructor */
  coverpoint() = default;

  /*!
   *  \brief Constructor that registers a new default bin
   */
  template <typename... Args>
  coverpoint(bin<T> n, Args... args) : coverpoint(args...)
  {
    if (!n.is_empty())
    {
      std::reverse(n.intervals.begin(), n.intervals.end());
      bins.push_back(n);
    }
  }

  /*!
   *  \brief Constructor that a bin array
   */
  template < typename... Args>
  coverpoint(bin_array<T> n, Args... args) : coverpoint(args...)
  {
    n.add_to_cvp(*this);
  }

  /*!
   *  \brief Constructor that registers a new illegal bin
   */
  template <typename... Args>
  coverpoint(illegal_bin<T> n, Args... args) : coverpoint(args...)
  {
    if (!n.is_empty())
    {
      std::reverse(n.intervals.begin(), n.intervals.end());
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
      std::reverse(n.intervals.begin(), n.intervals.end());
      ignore_bins.push_back(n);
    }
  }

  /*!
   *  \brief Constructor that takes the parent covergroup.
   */
  template <typename... Args>
  coverpoint(const std::string& cvp_name, Args... args) : coverpoint(args...)
  {
    this->name = cvp_name;
  }

public:
  // Initialization constructor. Needed for the use of COVERGROUP macro;
  coverpoint(const coverpoint<T>& rh) = default;
  /*
   * Move assignment operator. Needed for the use of COVERGROUP macro. This should not be
   * "manually" used!
   */
  coverpoint<T>& operator=(coverpoint<T>&& rh) {
    /*
     * It is very important that the only fields moved from the right hand are
     * the bins. This is a needed assumption which makes the COVERPOINT macro
     * syntax work!
     */
    this->bins = std::move(rh.bins);
    this->ignore_bins = std::move(rh.ignore_bins);
    this->illegal_bins = std::move(rh.illegal_bins);
    return *this;
  }

  ~coverpoint() = default;

  /*!
   * \brief Initializer list constructor that receives a list of bin (of any types,
   * even mixed) and registers all the bins in this coverpoint.
   */
  coverpoint(std::initializer_list<const bin_wrapper<T>> list) {
    for (const bin_wrapper<T> &bin_w : list) {
      bin_w.get_bin()->add_to_cvp(*this);
    }
  }

  template <typename... Args>
  coverpoint(cvg_base *parent_cvg, Args... args) : coverpoint(args...) {
    static_assert(forbid_type<cvg_base *, Args...>::value, "Coverpoint constructor accepts only 1 parent covergroup pointer!");
    // Because the way that delegated constructors work, the coverpoint arguments
    // processed in the reverse order, resulting in a reversed vector of bins.
    std::reverse(bins.begin(), bins.end());
    parent_cvg->register_cvp(this);

    // set strings here
    auto strings = parent_cvg->get_strings(this);

    this->sample_point = static_cast<T *>(std::get<0>(strings));
    this->name = std::get<1>(strings);
    this->sample_expression_str = std::get<2>(strings);
  }

  /*!
   *  Retrieves the number of regular bins
   */
  uint64_t size() {
    return bins.size();
  }

  void sample() 
  {
    if (has_sample_expression) {
      if (!has_sample_condition() || sample_condition())
         this->sample(sample_expression());
      else {
        // This is a fix so that crosses are not sampled if any of the coverpoints
        // used for crossing has a sample condition which is not met.
        this->last_sample_success = false;
        this->last_bin_index_hit = 0;
      }
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
    if (bins.empty()) // no bins defined
      return (option.weight == 0) ? 100 : 0;

    double res = 0;
    for (auto &bin : bins)
      res += (bin.get_hitcount() >= option.at_least);

    double real = res * 100 / bins.size();

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

    if (!bins.size())
    {
      total = 0;
      return (option.weight == 0) ? 100 : 0;
    }

    for (auto &bin : bins)
      res += (bin.get_hitcount() >= option.at_least);

    covered = res;
    double real = res * 100 / total;
    return (real >= this->option.goal) ? 100 : real;
  }

  /*!
   *  \brief Changes the instances name
   *  \param new_name New associated name
   */
  void set_inst_name(const std::string &new_name)
  {
    this->name = new_name;
  }

  /*!
   * \brief Enables sampling
   */
  void start()
  {
    collect = true;
  }

  /*!
   * \brief Enables sampling
   */
  void stop()
  {
    collect = false;
  }

  /*!
   * \brief print instance in UCIS XML format
   * \param stream Where to print
   */
  virtual void to_xml(std::ostream &stream) const
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
    for (auto &bin : illegal_bins)
      bin.to_xml(stream);
    for (auto &bin : ignore_bins)
      bin.to_xml(stream);

    stream << "</ucis:coverpoint>\n\n";
  }

};

} // namespace fc4sc

#endif /* FC4SC_COVERPOINT_HPP */

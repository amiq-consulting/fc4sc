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
 \file cross.hpp
 \brief Helper functions and interfaces

 This file contains helper functions and the base classes
 for bins, coverpoints and covergroups.
 */

#ifndef FC4SC_CROSS_HPP
#define FC4SC_CROSS_HPP

#include <map>
#include <tuple>

#include "fc4sc_base.hpp"
#include "fc4sc_bin.hpp"
#include "fc4sc_coverpoint.hpp"
#include "fc4sc_binsof.hpp"

namespace fc4sc
{

/*!
 * \brief Defines a class for crosses
 * \tparam Args Type of coverpoints being crossed
 */
template <typename... Args>
class cross : public cvp_base
{
  /*! Sampling switch */
  bool collect = true;

  /*! Total number of bins in this cross */
  uint64_t total_coverpoints = 0;

  /*!
   * \brief Helper function to recursively determine number of bins in a cross
   * \tparam Head Type of current processed coverpoint
   * \tparam Tail Rest of coverpoints
   * \param h Current coverpoint
   * \param t Rest of coverpoints
   * \returns number of bins in this cross
   */
  template <typename Head, typename... Tail>
  int get_size(Head h, Tail... t)
  {
    return h->size() * get_size(t...);
  }

  /*!
   * \brief End of recursion
   * \returns Default value 1
   */
  int get_size()
  {
    return 1;
  }

  /*!
   *  \brief Helper function to check if a sampled value is in a cross
   *  \tparam k Index of the currently checked element
   *  \tparam Head Type of current processed value
   *  \tparam Tail Rest of sampled values
   *  \param found Storage of hit bins for each coverpoint
   *  \param h Current sample value checked
   *  \param t Rest of sample values
   *  \returns True if each value is in its corresponding coverpoint
   */
  template <size_t k, typename Head, typename... Tail>
  bool check(std::vector<std::vector<size_t>> &found, Head h, Tail... t)
  {

    // See if the current value is in its coverpoint, and where
    std::vector<size_t> bin_indexes = (static_cast<coverpoint<Head> *>(cvps_vec[k - 1]))->get_bin_index(h);

    // If atleast one bin has it
    bool found_in_cvp = bin_indexes.size() > 0;

    if (!found_in_cvp)
    {
      return false;
    }

    // Store bins for this coverpoint
    found.push_back(bin_indexes);

    // Recurse
    return (k == 1) ? (found_in_cvp) : (check<k - 1>(found, t...) && (found_in_cvp));
  }

  /*!
   * \brief End of recursion
   */
  template <size_t k>
  bool check(std::vector<std::vector<size_t>> &found) {
    (void)found; // safe warning suppression
    return true;
  }

public:
  /*! Hit cross bins storage */
  std::map<std::vector<size_t>, uint64_t> bins;

  /*! Crossed coverpoints storage */
  std::vector<cvp_base *> cvps_vec;

  /*!
   *  \brief Main constructor
   *  \param n Parent covergroup
   *  \param args Coverpoints to be crossed
   */
  template <typename... Restrictions>
  cross(cvg_base *n, coverpoint<Args> *... args, Restrictions... binsofs) : cross(binsofs...) 
  {
    n->cvps.push_back(this);

    total_coverpoints = get_size(args...);
    cvps_vec = std::vector<cvp_base*>{args...};

    std::reverse(cvps_vec.begin(), cvps_vec.end());
  };

  
  template <typename... Restrictions, typename Select>
  cross(binsof<Select> binsof_inst,  Restrictions... binsofs) : cross (binsofs...) {
    std::cerr << "consume binsof\n";
    static_cast<void>(binsof_inst);
  }

  cross(cvg_base *n, const std::string& name, coverpoint<Args> *... args) : cross(n, args...) {
    this->name = name;
  };


  /*!
   *  \brief Default constructor
   */
  cross(){}

  /*!
   *  \brief Sampling function at cross level
   */
  virtual void sample() 
  {
    if (!this->collect) return;
    std::vector <size_t> hit_bins;
    for (auto& cvp : cvps_vec) {
      if (cvp->last_sample_success) {
        hit_bins.push_back(cvp->last_bin_index_hit);
      }
      else {
        misses++;
        return;
      }
    }
    bins[hit_bins]++;
  }

  /*!
   *  \brief Computes coverage for this instance
   *  \returns Coverage value as a double between 0 and 100
   */
  double get_inst_coverage() const 
  {

    int covered = 0;
    int total = total_coverpoints;

    if (total == 0)
      return (this->option.weight == 0) ? 100 : 0;

    for (auto it : bins)
    {
      if (it.second >= option.at_least)
        covered++;
    }

    double real = 100.0 * covered / total;
    return (real >= this->option.goal) ? 100 : real;
  }

  /*!
   *  \brief Computes coverage for this instance
   *  \param covered Number of covered bins
   *  \param total Total number of bins in this cross
   *  \returns Coverage value as a double between 0 and 100
   */
  double get_inst_coverage(int &covered, int &total) const 
  {
    total = total_coverpoints;
    covered = 0;

    for (auto it : bins)
    {
      if (it.second >= option.at_least)
        covered++;
    }

    if (total == 0)
      return (this->option.weight == 0) ? 100 : 0;

    return covered / total;
  }

  /*!
   *  \brief Changes the instances name
   *  \param new_name New associated name
   */
  void set_inst_name(const std::string &new_name)
  {
    name = new_name;
  };

  /*!
   * \brief Enables sampling
   */
  void start()
  {
    collect = true;
  };

  /*!
   * \brief Disables sampling
   */
  void stop()
  {
    collect = false;
  };

  /*!
   * \brief print instance in UCIS XML format
   * \param stream Where to print
   */
  void to_xml(std::ostream &stream) const
  {

    stream << "<ucis:cross ";
    stream << "name=\"" << this->name << "\" ";
    stream << "key=\""
           << "KEY"
           << "\" ";
    stream << ">\n";

    stream << option << "\n";

    for (auto &cvp : cvps_vec)
    {
      stream << "<ucis:crossExpr>" << cvp->name << "</ucis:crossExpr> \n";
    }

    for (auto& bin : bins)
    {
      stream << "<ucis:crossBin \n";
      stream << "name=\""
             << ""
             << "\"  \n";
      stream << "key=\"" << 0 << "\" \n";
      stream << "type=\""
             << "default"
             << "\" \n";
      stream << "> \n";

      for (auto& index : bin.first)
        stream << "<ucis:index>" << index << "</ucis:index>\n";

      stream << "<ucis:contents \n";
      stream << "coverageCount=\"" << bin.second << "\"> \n";
      stream << "</ucis:contents> \n";

      stream << "</ucis:crossBin> \n";
    }

    stream << "<ucis:userAttr \n";
    stream << "key=\"" << total_coverpoints << "\" \n";
    stream << "type=\""
           << "int"
           << "\" \n";
    stream << "/> \n";

    stream << "</ucis:cross>\n";

    return;
  };
};

} // namespace fc4sc

#endif /* FC4SC_CROSS_HPP */

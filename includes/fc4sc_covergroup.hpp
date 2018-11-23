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
 \file covergroup.hpp
 \brief Covergroup implementation

   This file contains the covergroup implementation
 with all the non-user functions
 */

#ifndef FC4SC_COVERGROUP_HPP
#define FC4SC_COVERGROUP_HPP

#include "fc4sc_base.hpp"
#include "fc4sc_master.hpp"

#include <unordered_map>
#include <typeinfo>
#include <tuple>

namespace fc4sc
{
/*!
 * \class covergroup covergroup.hpp
 * \brief Covergroup option declaration
 */
class covergroup : public cvg_base
{
protected:
  /*
   * This function registers a coverpoint instance inside this covergroup.
   * It receives as arguments a pointer to the coverpoint to be registered,
   * the coverpoint name, the sample expression lambda function and string,
   * and finally, the sample condition lambda function and string.
   * Finally, it returns a coverpoint constructed with the given arguments.
   * The purpose of this function is to be used for coverpoint instantiation
   * via the COVERPOINT macro and should not be explicitly used!
   */
  template<typename T>
  coverpoint <T> register_cvp(coverpoint <T>* cvp, std::string&& cvp_name,
    std::function<T()>&& sample_expr, std::string&& sample_expr_str,
    std::function<bool()>&& sample_cond, std::string&& sample_cond_str) {

    // NOTE: VERY important! Do not attempt to dereference the cvp pointer in
    // any way because it points to uninitialized memory!
    cvg_base::register_cvp(cvp);
    coverpoint<T> cvp_structure;
    cvp_structure.has_sample_expression = true;
    cvp_structure.sample_expression = sample_expr;
    cvp_structure.sample_condition = sample_cond;
    cvp_structure.sample_expression_str = sample_expr_str;
    cvp_structure.sample_condition_str = sample_cond_str;
    cvp_structure.name = cvp_name;
    return cvp_structure;
  }

  std::unordered_map<cvp_base *, cvp_metadata_t> cvp_strings;

   /*!
   * \brief Registers an instance and some info to \link fc4sc::global_access \endlink
   * \param type_name Type of covergroup stringified
   * \param file_name File of declaration
   * \param line  Line of declaration
   * \param inst_name Name of the instance
   */
  covergroup(const char *type_name, const char *file_name = "", int line = 0, const std::string &inst_name = "")
  {
    this->type_name = type_name;
    this->file_name = file_name;
    this->line = line;
    fc4sc::global::register_new(this, type_name, file_name, line, inst_name);
  }

  uint32_t set_strings(cvp_base *cvp, void *sample_point, const std::string& cvp_name, const std::string& expr_name) {
    cvp_strings[cvp] = cvp_metadata_t(sample_point, cvp_name, expr_name);
    return 0;
  }

  virtual cvp_metadata_t get_strings(cvp_base *cvp) {
    return cvp_strings[cvp];
  }

  /*! Disabled */
  covergroup() = delete;
  /*! Disabled */
  covergroup &operator=(const covergroup &other) = delete;
  /*! Disabled */
  covergroup(const covergroup &other) = delete;
  /*! Disabled */
  covergroup(covergroup &&other) = delete;
  // ! Disabled
  covergroup &operator=(covergroup &&other) = delete;

  /*! Destructor */
  virtual ~covergroup() { fc4sc::global::register_delete(this); }
public:

  virtual void sample() {
    for (auto& cvp : this->cvps) {
      try {
        cvp->sample();
      }
      catch(illegal_bin_sample_exception &e) {
        e.update_cvg_info(this->name);
        std::cerr << e.what() << std::endl;
#ifndef FC4SC_NO_THROW // By default the simulation will stop
        std::cerr << "Stopping simulation\n";
	throw(e);
#endif
      }
    }
  }

  /*!
   * \brief Computes coverage of an instance
   * \returns Coverage percentage of this instance
   */
  double get_inst_coverage()  const 
  {
    double res = 0;
    double weights = 0;

    for (auto &cvp : cvps) {
      res += cvp->get_inst_coverage() * cvp->option.weight;
      weights += cvp->option.weight;
    }

    if (weights == 0 || cvps.size() == 0 || res == 0)
      return (this->option.weight == 0) ? 100 : 0;

    double real = res / weights;
    return (real >= this->option.goal) ? 100 : real;
  }

  /*!
   * \brief Computes coverage of an instance
   * \param bins_covered Number of covered bins
   * \param total Total number of bins in this covergroup
   * \returns Coverage percentage of this instance
   */
  double get_inst_coverage(int &bins_covered, int &total) const 
  {
    double res = 0;
    double weights = 0;
    int bins_aux = 0;
    int total_aux = 0;

    bins_covered = 0;
    total = 0;

    for (auto &cvp : cvps)
    {
      res += cvp->get_inst_coverage(bins_aux, total_aux) * cvp->option.weight;
      bins_covered += bins_aux;
      total += total_aux;
      weights += cvp->option.weight;
    }

    if (weights == 0 || cvps.size() == 0 || res == 0)
    {
      total = 0;
      bins_covered = 0;
      return (this->option.weight == 0) ? 100 : 0;
    }

    double real = res / weights;
    return (real >= this->option.goal) ? 100 : real;
  }

  /*!
   *  \brief Computes coverage count across all instances of this type
   *  \returns Coverage percentage of this instance
   */
  double get_coverage()
  {
    return fc4sc::global::get_coverage(this->type_name);
  }

  /*!
   * \brief Computes coverage of an instance
   * \param bins_covered Number of covered bins
   * \param total Total number of bins in this covergroup
   * \returns Coverage percentage of this instance
   */
  double get_coverage(int &bins_covered, int &total)
  {
    return fc4sc::global::get_coverage(this->type_name, bins_covered, total);
  }

  /*!
   * \brief Enables sampling on all coverpoints/crosses
   */
  void start()
  {
    for (auto i : cvps)
      i->start();
  };

  /*!
   * \brief Enables sampling on all coverpoints/crosses
   */
  void stop()
  {

    for (auto i : cvps)
      i->stop();
  };

  /*!
   * \brief print instance in UCIS XML format
   * \param stream Where to print
   */
  virtual void to_xml(std::ostream &stream) const
  {

    stream << option << "\n";

    stream << "<ucis:cgId cgName=\"" << this->name << "\" ";
    stream << "moduleName=\""
           << "INST_PARENT_MODULE"
           << "\">\n";

    stream << "<ucis:cginstSourceId file=\""
           << "1"
           << "\" line=\""
           << "1"
           << "\" inlineCount=\"1\"/>\n";
    stream << "<ucis:cgSourceId file=\"" << file_name << "\" "
           << "line=\"" << line << "\""
           << " inlineCount=\"1\"/>\n";
    stream << "</ucis:cgId>\n";

    // Print coverpoints
    for (auto& cvp : cvps)
      cvp->to_xml(stream);

  }

  inline cvg_type_option &type_option()
  {
    return global::type_option(type_name);
  }
};

} // namespace fc4sc

#endif /* FC4SC_COVERGROUP_HPP */

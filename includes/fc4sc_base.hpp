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
 \file fc_base.hpp
 \brief Helper functions and interfaces

 This file contains helper functions and the base classes
 for bins, coverpoints and covergroups.
 */

#ifndef FC4SC_BASE_HPP
#define FC4SC_BASE_HPP

#include <iostream>
#include <string>
#include <exception>
#include <algorithm>
#include <tuple>
#include <vector>
#include <utility>

#include "fc4sc_options.hpp"

/*
 * Template meta-programming tool used for checking that a parameter
 * pack doesn't contain any argument which is convertible to a specified type.
 * This is used in the bin & coverpoint constructors together with a
 * static_assert in order to make sure that the user does not provide
 * multiple arguments which must be provided only once.
 * For bin -> the name
 * For coverpoint -> pointer to the parent covergroup
 */
template<typename forbidden, typename head, typename...tail> struct forbid_type {
  static bool constexpr value = forbid_type<forbidden, tail...>::value &&
    (!std::is_convertible<head, forbidden>::value);
};
template<typename forbidden, typename first> struct forbid_type<forbidden, first> :
  public std::__not_<std::is_convertible<first, forbidden>> {};

/*!
 * \brief Macro to register your covergroup.
 *
 * This macro expands to a constructor for your custom type, which calls a parent constructor
 * to register the new instance, with some additional info useful for displaying the final report.
 */
#define CG_CONS(type, args...) \
  using covergroup::sample; \
  type(std::string inst_name = "", ##args) : fc4sc::covergroup(#type, __FILE__, __LINE__, inst_name)


/*!
 * \brief Macro to declare a sampling point variable and register some names
 */
#define SAMPLE_POINT(variable_name, cvp) variable_name = static_cast<decltype(variable_name)> \
  (covergroup::set_strings(&cvp, &variable_name, #cvp, #variable_name));

/*
 * \brief Macro that creates a lambda function which returns the expression given
 * as argument when called. A pointer to the enclosing class will be bound in the
 * lambda capture so that local class variables can be used in the lambda body.
 * The purpose of this macro is to use in the instantiation of coverpoints inside
 * covergroups and will serve as a generator for lambda expressions to be used
 * when sampling (sample expression and sample conditions).
 */
#define CREATE_WRAP_F(expr, type) [this]() -> type { return (expr); }

#define GET_CVP_MACRO(_1,_2,_3,_4, NAME,...) NAME

/*
 * Macro used to define and automatically register a coverpoint inside a covergroup.
 * The macro accepts 3 or 4 arguments in the following order:
 * 1) Type: used for the values of the bins declared in the coverpoint
 * 2) Name: the name of the coverpoint
 * 3) Sample Expression: the expression that will be used for sampling this coverpoint
 * whenever the covergroup is sampled (the type has to be the same as Type argument)
 * 4) Sample Condition: a condition that has to be true in order for this coverpoint
 * to be sampled when the covergroup is sampled (the type has to be bool)
 */
#define COVERPOINT(...) GET_CVP_MACRO(__VA_ARGS__, CVP_4, CVP_3)(__VA_ARGS__)

// COVERPOINT macro for 3 arguments (no sample condition)
#define CVP_3(type, cvp_name, sample_expr) \
        coverpoint<type> cvp_name = \
        covergroup::register_cvp<type>(&cvp_name, #cvp_name, \
        CREATE_WRAP_F(sample_expr, type), #sample_expr, \
        CREATE_WRAP_F(true, bool), std::string("")) =

// COVERPOINT macro for 4 arguments (sample condition included)
#define CVP_4(type, cvp_name, sample_expr, sample_cond) \
        coverpoint<type> cvp_name = \
        covergroup::register_cvp<type>(&cvp_name, #cvp_name, \
        CREATE_WRAP_F(sample_expr, type), #sample_expr, \
        CREATE_WRAP_F(sample_cond, bool), #sample_cond) =

/*!
 *  \namespace fc4sc
 *  \brief Namespace containing the library
 */
namespace fc4sc
{
using cvp_metadata_t = std::tuple<void*, std::string, std::string>;

typedef enum fc4sc_format {
  ucis_xml = 1
} fc4sc_format;

/*!
 *  \brief Alias to std::make_pair
 *  \param t1 One end of the interval
 *  \param t2 Other end of the interval
 *  \returns Pair of t1 and t2
 *
 *    Just a wrapper over make_pair to declare value intervals
 *  in a more verbose way.
 */
template <typename T>
static interval_t<T> interval(T t1, T t2) {
  return interval_t<T>(t1, t2);
}

/*!
 *  \class bin_base fc_base.hpp
 *  \brief Base class for bins
 *
 *  Basic interface to work with untemplated bins
 */
class bin_base
{
public:
  /*!
   * \brief Function to print a bin to UCIS XML
   * \param stream Where to print
   */
  virtual void to_xml(std::ostream &stream) const = 0;
  /*!
   * \brief Returns total number of hits
   * \returns number of times the sampled value was in the bin
   */
  virtual uint64_t get_hitcount() const = 0;
  /*! Destructor */
  virtual ~bin_base(){}
};

class api_base
{
public:
  std::string name;

  /** \name Coverage API
   *  API for getting and controlling coverage collection at run time
   */
  /**@{*/

  /*!
   * \brief Returns the coverage associated with this instance
   * \returns Double between 0 and 100
   */
  virtual double get_inst_coverage() const = 0;

  /*!
   * \brief Returns the coverage associated with this instance
   * \param hit Number of hit bins in this instance
   * \param total Total number of bins in this instance
   * \returns Double between 0 and 100
   */
  virtual double get_inst_coverage(int &hit, int &total) const = 0;

  /*!
   * \brief Changes the name of the instance
   * \param new_name The new name of the instance
   */
  virtual void set_inst_name(const std::string &new_name) {
    this->name = new_name;
  }

  /*!
   * \brief Enables sampling on this instance
   */
  virtual void start() = 0;

  /*!
   * \brief Stops sampling on this instance
   */
  virtual void stop() = 0;
  /**@}*/

  /*!
    * \brief Function to print an item to UCIS XML
    * \param stream Where to print
    */
  virtual void to_xml(std::ostream &stream) const = 0;

  virtual void sample() = 0;

  /*! Destructor */
  virtual ~api_base(){}
};

/*!
 *  \class cvp_base fc_base.hpp
 *  \brief Base class for coverpoints and crosses
 *
 *  Basic interface to work with untemplated coverpoints and crosses
 */
class cvp_base : public api_base
{
protected:
  bool stop_sample_on_first_bin_hit = false;

public:
  size_t last_bin_index_hit = 0;
  bool last_sample_success = false;

  /*! Instance specific options */
  cvp_option option;

  /*! Type specific options */
  cvp_type_option type_option;

  /*! Number of sample misses (no bin hit)*/
  uint64_t misses = 0;

  /*! Destructor */
  virtual ~cvp_base(){}
};

/*!
 *  \class cvg_base fc_base.hpp
 *  \brief Base class for covergroups
 *
 *  Basic interface to work with covergroups
 */
class cvg_base : public api_base
{
public:
  virtual void register_cvp(cvp_base* const cvp) {
    // Runtime check to make sure that all the coverpoints are different.
    // Normally, this should be the case, but if for whatever reason this
    // function is used multiple times with pointers to the the same coverpoint,
    // we make sure that the damage is minimal at least.
    if (std::find(std::begin(cvps), std::end(cvps), cvp) != std::end(cvps))
      throw ("Coverpoint already registered in this covergroup!");
    cvps.push_back(cvp);
  }

  /*! Retrieve sampling point, instance name and sampling point name */ 
  virtual cvp_metadata_t get_strings(cvp_base *cvp) = 0;

  /*! Array of associated coverpoints and crosses */
  std::vector<cvp_base *> cvps;

  /*! Instance specific options */
  cvg_option option;

  /*! Type specific options */
  cvg_type_option type_option;

  /*! Name of the covergroup type */
  std::string type_name;

  /*! File where this type is declared */
  std::string file_name;

  /*! Aproximate line where this type is declared */
  uint32_t line;
};

class illegal_bin_sample_exception : public std::exception {
private:
  std::string strconcat(const std::string& last) const { return last; }
  template <typename... Args>
  std::string strconcat(const std::string& head, Args... args) const {
    return head + strconcat(args...);
  }

  std::string value_hit;
  std::string bin_name;
  std::string cvp_name;
  std::string cvg_name;
  std::string message;

  void update_message() {
    message = strconcat("Illegal sample in [", cvg_name, "/", cvp_name, "/", bin_name, "] on value [", value_hit, "]!");
  }
public:
  void update_bin_info(const std::string& bin_name, const std::string& value) {
    this->bin_name = bin_name;
    this->value_hit = value;
    update_message();
  }
  void update_cvp_info(const std::string& cvp_name) {
    this->cvp_name = cvp_name;
    update_message();
  }
  void update_cvg_info(const std::string& cvg_name) {
    this->cvg_name = cvg_name;
    update_message();
  }

  const char * what () const throw () {
    return message.c_str();
  }
};
} // namespace fc4sc

#endif /* FC4SC_BASE_HPP */

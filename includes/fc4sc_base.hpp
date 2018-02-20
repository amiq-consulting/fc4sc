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
#include <algorithm>
#include <tuple>
#include <vector>
#include <utility>

#include "fc4sc_options.hpp"

using std::tuple;

using std::make_pair;
using std::ostream;
using std::pair;
using std::string;
using std::vector;

/*!
 * \brief Macro to register your covergroup.
 *
 *  This macro expands to a constructor for your custom type, which calls a parent constructor
 * to register the new instance, with some aditional info useful for displaying the final report.
 */
#define CG_CONS(type, args...) \
  type(string inst_name = "", ##args) : fc4sc::covergroup(#type, __FILE__, __LINE__, inst_name)


/*!
 * \brief Macro to declare a sampling point variable and register some names
 *
 */
#define SAMPLE_POINT(variable_name, cvp)                                        \
  variable_name;                                                                \
  bool init_##cvp = this->set_strings(&cvp, &variable_name, #cvp, #variable_name);              \

/*!
 *  \namespace fc4sc
 *  \brief Namespace containing the library
 */
namespace fc4sc
{


  typedef enum fc4sc_format 
  {
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
static pair<T, T> interval(T t1, T t2)
{
  return make_pair(t1, t2);
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
  virtual void to_xml(ostream &stream) const = 0;

  /*!
   * \brief Returns total number of hits
   * \returns number of times the sampled value was in the bin
   */
  virtual uint64_t get_hitcount() const = 0;
  // {
  //   return hits;
  // }

  /*! Destructor */
  virtual ~bin_base(){};
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
  virtual void set_inst_name(const string &new_name)
  {
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
  virtual void to_xml(ostream &stream) const = 0;

  virtual void sample() = 0;

};

/*!
 *  \class cvp_base fc_base.hpp
 *  \brief Base class for coverpoints and crosses
 *
 *  Basic interface to work with untemplated coverpoints and crosses
 */
class cvp_base : public api_base
{

public:

  uint last_bin_index_hit;
  uint last_sample_success;

  /*! The sampled expression stringified */
  string expr_str;

  /*! Name of the enclosing covergroup */
  string p_name;

  /*! If strings have been initialized */
  bool init = false;

  /*! Instance specific options */
  cvp_option option;

  /*! Type specific options */
  cvp_type_option type_option;

  /*! Number of samples not found in any bin */
  uint64_t misses;

  /*! Destructor */
  virtual ~cvp_base(){};

  /*! Sets string members to given values
   *  \param name Name of the coverpoint (if another isn't already assigned)
   *  \param p_name Name of the enclosing covergroup
   *  \param expr Name of the arguments given to sample
   */
  virtual bool set_strings(const string &name, const string &p_name, const string &expr)
  {

    if (this->name.empty())
      this->name = name;

    this->expr_str = expr;
    this->p_name = p_name;
    this->init = true;

    return true;
  }

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

  /*! Retrieve sampling point, instance name and sampling point name */ 
  virtual tuple<void*, string, string> get_strings(cvp_base *cvp) = 0;  

  /*! Array of associated coverpoints and crosses */
  vector<cvp_base *> cvps;

  /*! Instance specific options */
  cvg_option option;

  /*! Type specific options */
  cvg_type_option type_option;

  /*! Name of the covergroup type */
  string type_name;

  /*! File where this type is declared */
  string file_name;

  /*! Aproximate line where this type is declared */
  unsigned int line;

};

} // namespace fc4sc

#endif /* FC4SC_BASE_HPP */

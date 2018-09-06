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

using std::unordered_map;

using std::tuple;
using std::make_tuple;

namespace fc4sc
{

/*!
 * \class covergroup covergroup.hpp
 * \brief Covergroup option declaration
 */
class covergroup : public cvg_base
{

  unordered_map<cvp_base *, tuple<void*, string, string> > cvp_strings;

protected:

   /*!
   * \brief Registers an instance and some info to \link fc4sc::global_access \endlink
   * \param type_name Type of covergroup stringified
   * \param file_name File of declaration
   * \param line  Line of declaration
   * \param inst_name Name of the instance
   */
  covergroup(const char *type_name, const char *file_name = "", int line = 0, const string &inst_name = "")
  {
    this->type_name = type_name;
    this->file_name = file_name;
    this->line = line;

    fc4sc::global::register_new(this, type_name, file_name, line, inst_name);
  }


public:


  bool set_strings(cvp_base *cvp, void *sample_point, const string& cvp_name, const string& expr_name) {

    cvp_strings[cvp] = make_tuple(sample_point, cvp_name, expr_name);
    return true;
  }

  virtual tuple<void*, string, string> get_strings(cvp_base *cvp) {
    return cvp_strings[cvp];
  }
 
  covergroup() = default;

  /*! Destructor */
  virtual ~covergroup()
  {
    fc4sc::global::register_delete(this);
  }

  /*! Disabled */
  virtual covergroup &operator=(const covergroup &other) = delete;

  /*! Disabled */
  covergroup(const covergroup &other) = delete;

  // /*! Disabled */
  covergroup(covergroup &&other) = delete;

  // ! Disabled
  covergroup &operator=(covergroup &&other) = delete;

  void sample() {
    for (auto& cvp : this->cvps) 
      cvp->sample();
  }

  /*!
   * \brief Computes coverage of an instance
   * \returns Coverage percentage of this instance
   */
  double get_inst_coverage()  const 
  {

    double res = 0;
    double weights = 0;

    for (auto &cvp : cvps)
    {
      res += cvp->get_inst_coverage() * cvp->option.weight;
      weights += cvp->option.weight;
    }

    if (weights == 0 || cvps.size() == 0 || res == 0)
    {

      if (this->option.weight != 0)
        return 0;
      else
        return 100;
    }

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

      if (this->option.weight != 0)
        return 0;
      else
        return 100;
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
   *  \brief Changes the instances name
   *  \param new_name New associated name
   */
  void set_inst_name(const string &new_name)
  {
    name = new_name;
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
  virtual void to_xml(ostream &stream) const
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
    stream << "<ucis:cgSourceId file=\""
           << "2"
           << "\" line=\""
           << "3"
           << "\" inlineCount=\"1\"/>\n";
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

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
 \file fc_master.hpp
 \brief Global overseer

   This file contains a global table of covergroups and a "static" wrapper class
 for it. All type specific computation and report writing is done from here.
 */

#ifndef FC4SC_MASTER_HPP
#define FC4SC_MASTER_HPP

#include <unordered_map>
#include <fstream>
#include <stdint.h>

#include "fc4sc_base.hpp"

namespace fc4sc
{

/*!
 * \class global fc_master.hpp
 * \brief Static proxy to a \link fc4sc::main_controller \endlink instance
 */
class global
{
  /*!
   * \class main_controller fc_master.hpp
   * \brief Keeps evidence of all the covergroup instances and types
   */
  class main_controller
  {
    // Inner struct for holding data on each type
    struct metadata
    {
      cvg_type_option type_option;
      std::string type_name;
      std::string file_name;
      uint32_t line;
      uint32_t index = 0;


      std::vector<cvg_base *> cvg_insts;
      std::vector<std::string> cvg_insts_name;

      std::string get_next_name()
      {
        return type_name + "_" + std::to_string((unsigned long long)(++index));
      }

      friend std::ostream &operator<<(std::ostream &stream, const metadata &inst)
      {
        stream << "type_name: [" << inst.type_name << "]\n";
        stream << "file_name: [" << inst.file_name << "]\n";
        stream << "line     : [" << inst.line << "]\n";

        for (size_t i = 0; i < inst.cvg_insts_name.size(); ++i)
          stream << "\tinst_name : [" << inst.cvg_insts[i] << "][" << inst.cvg_insts_name[i] << "]\n";

        return stream;
      }
    };

    /*! Table keeping instances data grouped by type */
    std::unordered_map<std::string, metadata> cv_data;

  public:
    /*!
     * \brief Adds a new instance to the global table
     * \param type_name Type of covergroup stringified
     * \param file_name File of declaration
     * \param line  Line of declaration
     * \param inst_name Name of the instance
   */
    void register_new(cvg_base *cvg,
                      const std::string &type_name,
                      const std::string &file_name,
                      const int line,
                      const std::string &inst_name = "")
    {
      // New type registered
      if (cv_data.find(type_name) == cv_data.end())
      {
        // Store location info
        metadata temp;

        temp.type_name = type_name;
        temp.file_name = file_name;
        temp.line = line;

        cv_data[type_name] = temp;
      }

      // Add the instance
      cv_data[type_name].cvg_insts.push_back(cvg);

      // Add the name
      if (inst_name.empty())
      {
        cvg->name = cv_data[type_name].get_next_name();
        cv_data[type_name].cvg_insts_name.push_back(cvg->name);
      }
      else
      {
        cvg->name = inst_name;
        cv_data[type_name].cvg_insts_name.push_back(inst_name);
      }

    }

    void register_delete(cvg_base *cvg)
    {
      // cvg->type_name
      std::vector<cvg_base *> &cvgs = cv_data[cvg->type_name].cvg_insts;
      std::vector<std::string> &names = cv_data[cvg->type_name].cvg_insts_name;

      auto it = find(cvgs.begin(), cvgs.end(), cvg);
      int index = it - cvgs.begin();

      cvgs.erase(cvgs.begin() + index);
      names.erase(names.begin() + index);

      if (cvgs.empty())
      { // just like the type never existed
        cv_data.erase(cvg->type_name);
      }

    }

    /*!
     * \brief Function called to print an UCIS XML with all the data
     * \param stream Where to print
     */
    std::ostream &print_data_xml(std::ostream &stream)
    {

      // Header
      stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
             << "\n";
      stream << "<ucis:UCIS xmlns:ucis=\"http://www.w3.org/2001/XMLSchema-instance\" ucisVersion=\"1.0\" ";
      stream << " writtenBy=\""
             << "$USER"
             << "\"\n";
      stream << " writtenTime=\""
             << "2008-09-29T03:49:45"
             << "\"";
      stream << ">\n";

      // Needed information but not filled
      stream << "<ucis:sourceFiles ";
      stream << " fileName=\""
             << "string"
             << "\"";
      stream << " id=\""
             << "201"
             << "\" ";
      stream << "/>\n";

      // Needed information but not filled
      stream << "<ucis:historyNodes ";
      stream << "historyNodeId=\"" << 200 << "\" \n";
      stream << "parentId=\"" << 200 << "\" \n";
      stream << "logicalName=\""
             << "string"
             << "\" \n";
      stream << "physicalName=\""
             << "string"
             << "\" \n";
      stream << "kind=\""
             << "string"
             << "\" \n";
      stream << "testStatus=\""
             << "true"
             << "\" \n";
      stream << "simtime=\""
             << "1.051732E7"
             << "\" \n";
      stream << "timeunit=\""
             << "string"
             << "\" \n";
      stream << "runCwd=\""
             << "string"
             << "\" \n";
      stream << "cpuTime=\""
             << "1.051732E7"
             << "\" \n";
      stream << "seed=\""
             << "string"
             << "\" ";
      stream << "cmd=\""
             << "string"
             << "\" \n";
      stream << "args=\""
             << "string"
             << "\" ";
      stream << "compulsory=\""
             << "string"
             << "\" \n";
      stream << "date=\""
             << "2004-02-14T19:44:14"
             << "\" \n";
      stream << "userName=\""
             << "string"
             << "\" \n";
      stream << "cost=\""
             << "1000.00"
             << "\" \n";
      stream << "toolCategory=\""
             << "string"
             << "\" \n";
      stream << "ucisVersion=\""
             << "string"
             << "\" \n";
      stream << "vendorId=\""
             << "string"
             << "\" \n";
      stream << "vendorTool=\""
             << "string"
             << "\" \n";
      stream << "vendorToolVersion=\""
             << "string"
             << "\" \n";
      stream << "sameTests=\""
             << "42"
             << "\" ";
      stream << "comment=\""
             << "string"
             << "\" \n";
      stream << ">\n";
      stream << "</ucis:historyNodes>\n";

      static int key = 0;

      // Each type is between an instanceCoverages tag
      for (auto &type_it : cv_data)
      {

        stream << "<ucis:instanceCoverages ";
        stream << "name=\""
               << "string"
               << "\" \n";
        stream << "key=\"" << ++key << "\" \n";
        stream << "instanceId=\"" << ++key << "\" \n";
        stream << "alias=\""
               << "string"
               << "\" \n";
        stream << "moduleName=\"" << type_it.second.type_name << "\" \n";
        stream << "parentInstanceId=\"" << 0 << "\" \n";
        stream << ">\n";

        stream << "<ucis:id ";
        stream << "file=\"" << type_it.second.file_name << "\" ";
        stream << "line=\"" << type_it.second.line << "\" ";
        stream << "inlineCount=\""
               << "1"
               << "\" ";
        stream << "/>\n";

        stream << "<ucis:covergroupCoverage ";
        stream << "weight=\"" << 1 << "\" ";
        stream << ">\n";

        // Print each instance
        for (size_t i = 0; i < type_it.second.cvg_insts.size(); ++i)
        {

          stream << "<ucis:cgInstance ";
          stream << "name=\"" << type_it.second.cvg_insts_name[i] << "\" \n";

          stream << "key=\"" << ++key << "\" \n";
          stream << "alias=\""
                 << "string"
                 << "\" \n";
          stream << "excluded=\""
                 << "false"
                 << "\" \n";
          stream << ">\n";

          // stream << (*(type_it.second.cvg_insts[i])) << "\n";
          type_it.second.cvg_insts[i]->to_xml(stream);
          stream << "\n";
          stream << "</ucis:cgInstance>\n";
        }

        stream << "</ucis:covergroupCoverage>\n";

        stream << "</ucis:instanceCoverages>\n";
      }

      stream << "</ucis:UCIS>\n";

      key = 0;
      return stream;
    };

    /*!
   * \brief Computes the coverage percentage across all instances of all types
   * \returns Double between 0 and 100
   */
    double get_coverage()
    {
      double res = 0;
      double weights = 0;

      for (auto &types : cv_data) {
        res += get_coverage(types.first) * types.second.type_option.weight;
        weights += types.second.type_option.weight;
      }

      if (cv_data.size() == 0) return 100;
      if (weights == 0)        return 0;
      return res / weights;
    };

    /*!
   * \brief Computes the coverage percentage across all instances of a given type
   * \param type Unmangled type name
   * \returns Double between 0 and 100
   */
    double get_coverage(const std::string &type)
    {
      std::vector<cvg_base *> cvgs = cv_data[type].cvg_insts;

      double res = 0;
      double weights = 0;

      for (auto it : cvgs)
      {
        res += it->get_inst_coverage() * it->option.weight;
        weights += it->option.weight;
      }

      if (weights == 0 || cvgs.size() == 0 || res == 0)
        return (cv_data[type].type_option.weight == 0) ? 100 : 0;

      double real = res / weights;
      return (real >= cv_data[type].type_option.goal) ? 100 : real;
    };

    /*!
     * \brief Computes the coverage percentage across all instances of a given type
     * \param type Unmangled type name
     * \param hit_bins Total number of hit bins in instances
     * \param total_bins Total number of bins in instances
     * \returns Double between 0 and 100
     */
    // TODO merge hit_bins
    double get_coverage(const std::string &type, int &hit_bins, int &total_bins)
    {
      std::vector<cvg_base *> cvgs = cv_data[type].cvg_insts;

      double res = 0;
      double weights = 0;

      int hit_bins_inst = 0;
      int total_bins_inst = 0;

      for (auto it : cvgs)
      {
        res += it->get_inst_coverage(hit_bins_inst, total_bins_inst) * it->option.weight;
        hit_bins += hit_bins_inst;
        total_bins += total_bins_inst;

        weights += it->option.weight;
      }

      if (weights == 0 || cvgs.size() == 0 || res == 0 || total_bins == 0)
      {
        hit_bins = 0;
        total_bins = 0;
        return (cv_data[type].type_option.weight == 0) ? 100 : 0;
      }

      double real = res / weights;
      return (real >= cv_data[type].type_option.goal) ? 100 : real;
    };

    cvg_type_option &type_option(const std::string &type)
    {
      return cv_data[type].type_option;
    }

    bool empty() const {
      return cv_data.empty();
    }

  };


  /*!
   * \brief Manages the \link fc4sc::main_controller \endlink global instance
   */
  static main_controller *getter()
  {
    static main_controller *global = new main_controller;
    return global;
  }

public:
  /*!
   * \brief Adds a new instance to the global table
   * \param cvg Pointer to the newly instantiated covergroup
   * \param type_name Type of covergroup stringified
   * \param file_name File of declaration
   * \param line  Line of declaration
   * \param inst_name Name of the instance
   */
  static void register_new(cvg_base *cvg,
			   const std::string &type_name,
			   const std::string &file_name,
                           const int line,
			   const std::string &inst_name = "")
  {
    main_controller *global = getter();
    global->register_new(cvg, type_name, file_name, line, inst_name);
  }

  static void register_delete(cvg_base *cvg)
  {
    main_controller *global = getter();
    global->register_delete(cvg);
  }

  static cvg_type_option &type_option(const std::string &type)
  {
    main_controller *global = getter();
    return global->type_option(type);
  }

  /*!
  * \brief Computes the coverage percentage across all instances of all types
  * \returns Double between 0 and 100
  */
  static double get_coverage()
  {
    main_controller *global = getter();
    return global->get_coverage();
  }

  /*!
   * \brief Computes the coverage percentage across all instances of a given type
   * \param type Unmangled type name
   * \returns Double between 0 and 100
   */
  static double get_coverage(const std::string &type)
  {
    main_controller *global = getter();
    return global->get_coverage(type);
  }

  /*!
   * \brief Computes the coverage percentage across all instances of a given type
   * \param type Unmangled type name
   * \param hit_bins Total number of hit bins across instances of same type
   * \param total_bins Total number of bins across instances of same type
   * \returns Double between 0 and 100
   */
  static double get_coverage(const std::string &type, int &hit_bins, int &total_bins)
  {
    main_controller *global = getter();
    return global->get_coverage(type, hit_bins, total_bins);
  }

  /*!
   * \brief Prints data to the given file name
   * \param file_name Where to print. Returns if empty
   */
  static void coverage_save(const std::string &file_name = "", const fc4sc_format how = fc4sc_format::ucis_xml)
  {
    if (file_name.empty()) {
      std::cerr << "FC4SC " << __FUNCTION__ << " was passed empty string\n";
      std::cerr << "\tReturning\n";
      return;
    }

    main_controller *global = getter();
    switch(how) {
      case fc4sc_format::ucis_xml: {
        std::ofstream f(file_name);
        global->print_data_xml(f);
        break;
      }
      default :
        break;
    }
  }

  /*!
   * \brief Prints data to the given std::stream object.
   * \param stream object where to print.
   */
  static void coverage_save(std::ostream &stream, const fc4sc_format how = fc4sc_format::ucis_xml)
  {
    main_controller *global = getter();
    switch(how) {
      case fc4sc_format::ucis_xml: {
	global->print_data_xml(stream);
	break;
      }
      default :
	break;
    }
  }
};

} // namespace fc4sc
#endif /* FC4SC_MASTER_HPP */

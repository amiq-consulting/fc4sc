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
 \file fc_options.hpp
 \brief Structures for options and type options

   This file contains the definitions for the configurable options
 of coverage objects
 */

#ifndef FC4SC_OPTIONS_HPP
#define FC4SC_OPTIONS_HPP

#include <string>
#include <ostream>

/*!
 * \class cvg_option fc_options.hpp
 * \brief Covergroup option declaration
 */
struct cvg_option
{
  /*! Weight of instance when computing coverage */
  uint weight;

  /*! Target coverage percentage */
  uint goal;

  /*! Comment for this coverage */
  std::string comment;

  /*! Minimum of hits for each bin */
  uint at_least;

  /*! !UNIPLEMENTED! Max number of generated bins */
  uint auto_bin_max;

  /*! !UNIPLEMENTED! Issue warning if bins overlap in cvp */
  bool detect_overlap;

  /*! !UNIPLEMENTED! Number of missing bins in crosses to be printed */
  uint cross_num_print_missing;

  /*! !UNIPLEMENTED! Collect coverage per instance */
  bool per_instance;

  /*! !UNIPLEMENTED! Enables covergroup::get_inst_coverage() */
  bool get_inst_coverage;

  /*!
   * \brief Sets all values to default
   */
  cvg_option()
  {
    this->weight = 1;
    this->goal = 100;
    this->at_least = 1;
    this->auto_bin_max = 10;
    this->detect_overlap = 0;

    this->cross_num_print_missing = 0;
    this->per_instance = 0;
    this->get_inst_coverage = 0;
  }

  /*!
   * \brief Prints option in UCIS XML format
   */
  friend inline std::ostream &operator<<(std::ostream &stream, const cvg_option &inst)
  {
    stream << "<ucis:options ";
    stream << "weight=\"" << inst.weight << "\" ";
    stream << "goal=\"" << inst.goal << "\" ";
    stream << "comment=\"" << inst.comment << "\" ";
    stream << "at_least=\"" << inst.at_least << "\" ";
    stream << "auto_bin_max=\"" << inst.auto_bin_max << "\" ";
    stream << "detect_overlap=\"" << inst.detect_overlap << "\" ";
    stream << "cross_num_print_missing=\"" << inst.cross_num_print_missing << "\" ";
    stream << "per_instance=\"" << inst.per_instance << "\" ";
    stream << "/>";
    return stream;
  }
};

/*!
 * \class cvp_option fc_options.hpp
 * \brief Coverpoint option declaration
 */
struct cvp_option
{
  /*! Weight of instance when computing coverage */
  uint weight;

  /*! Target coverage percentage */
  uint goal;

  /*! Comment for this coverage */
  std::string comment;

  /*! Minimum of hits for each bin */
  uint at_least;

  /*! !UNIPLEMENTED! Max number of generated bins */
  uint auto_bin_max;

  /*! !UNIPLEMENTED! Issue warning if bins overlap in cvp */
  bool detect_overlap;

  /*!
   * \brief Sets all values to default
   */
  cvp_option()
  {
    this->weight = 1;
    this->goal = 100;
    this->at_least = 1;
    this->auto_bin_max = 10;
    this->detect_overlap = 0;
  }

  /*!
   * \brief Prints option in UCIS XML format
   */
  friend std::ostream &operator<<(std::ostream &stream, const cvp_option &inst)
  {
    stream << "<ucis:options ";
    stream << "weight=\"" << inst.weight << "\" ";
    stream << "goal=\"" << inst.goal << "\" ";
    stream << "comment=\"" << inst.comment << "\" ";
    stream << "at_least=\"" << inst.at_least << "\" ";
    stream << "auto_bin_max=\"" << inst.auto_bin_max << "\" ";
    stream << "detect_overlap=\"" << inst.detect_overlap << "\" ";
    stream << "/>";

    return stream;
  }
};

/*!
 * \class cross_option fc_options.hpp
 * \brief Covergroup option declaration
 */
struct cross_option // cross option declaration
{
  /*! Weight of instance when computing coverage */
  uint weight;

  /*! Target coverage percentage */
  uint goal;

  /*! Comment for this coverage */
  std::string comment;

  /*! Minimum of hits for each bin */
  uint at_least;

  /*! !UNIPLEMENTED! Number of missing bins in crosses to be printed */
  uint cross_num_print_missing;

  /*!
   * \brief Sets all values to default
   */
  cross_option()
  {
    weight = 1;
    goal = 100;
    at_least = 1;
    cross_num_print_missing = 0;
  }

  /*!
   * \brief Prints option in UCIS XML format
   */
  friend std::ostream &operator<<(std::ostream &stream, const cross_option &inst)
  {
    stream << "<ucis:options ";
    stream << "weight=\"" << inst.weight << "\" ";
    stream << "goal=\"" << inst.goal << "\" ";
    stream << "comment=\"" << inst.comment << "\" ";
    stream << "at_least=\"" << inst.at_least << "\" ";
    stream << "cross_num_print_missing=\"" << inst.cross_num_print_missing << "\" ";
    stream << "/>";

    return stream;
  }
};

/*!
 * \class cvg_type_option fc_options.hpp
 * \brief Covergroup option declaration
 */
struct cvg_type_option // covergroup type_option declaration
{
  /*! Weight of instance when computing coverage */
  uint weight;

  /*! Target coverage percentage */
  uint goal;

  /*! Comment for this coverage */
  std::string comment;

  /*! TODO: !UNIPLEMENTED! Computes coverage cumulatively across all instances */
  bool merge_instances;

  /*!
   * \brief Sets all values to default
   */
  cvg_type_option()
  {
    this->weight = 1;
    this->goal = 100;
    this->merge_instances = 0;
  }
};

/*!
 * \class cvp_type_option fc_options.hpp
 * \brief Covergroup option declaration
 */
struct cvp_type_option // coverpoint and cross type_option declaration
{
  /*! Weight of instance when computing coverage */
  uint weight;

  /*! Target coverage percentage */
  uint goal;

  /*! Comment for this coverage */
  std::string comment;

  /*!
   * \brief Sets all values to default
   */
  cvp_type_option()
  {
    this->weight = 1;
    this->goal = 100;
  }
};

#endif /* FC4SC_OPTIONS_HPP */

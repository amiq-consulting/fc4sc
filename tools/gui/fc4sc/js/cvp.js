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

function get_proc_style (excluded, pass, fail) {

  if (excluded)
    return "class=\"proc proc_excluded\"";

  if (pass == (pass + fail))
    return "class=\"proc proc_full\"";

  if (pass == 0)
    return "class=\"proc proc_empty\"";      

  return "class=\"proc proc_partial\"";

}

function Coverpoint(node, pass_bin, fail_bin, global_bin_tree) {

  cvp_bin_count = pass_bin + fail_bin;
  cvp_procent = pass_bin / (pass_bin + fail_bin);

  // ===============================================
  // cvp_procs.push((cvp_procent * 100).toFixed(2));
  // cvp_weights.push(option_node.getAttribute('weight'));
  // cg_inst_lines += pass_bin + fail_bin;
  // ===============================================

  var option_node;

  for (var i =0; i < node.childNodes.length; ++i ) {
    if (node.childNodes[i].nodeType == 1 && node.childNodes[i].nodeName == "ucis:options") {
      option_node = node.childNodes[i];
      break;
    }
  }

  this.name = node.getAttribute('name');
  this.exprString = node.getAttribute('exprString');
  this.cvp_bin_count = cvp_bin_count + 1;
  this.cvp_procent = (cvp_procent * 100).toFixed(2);
  this.cvp_weight = option_node.getAttribute('weight');
  this.cvp_bins = global_bin_tree;

  this.style = get_proc_style(false, pass_bin, fail_bin);

  this.cg_inst_lines = pass_bin + fail_bin + 2;
}

Coverpoint.prototype.stringify = function() {

  var result = "";

  result += "<tr>" 
  +    "<td " + this.style + " rowspan=" + (this.cvp_bin_count + 1) + ">" 
  +      this.cvp_procent + "%"
  +    "</td>"
  +  "</tr>"; 

  result += "<tr>" 
  +    "<td class=\"cvp\" rowspan=\"1\" colspan=\"5\">"
  +      this.name + " \"" + this.exprString + "\""
  +    "</td>"
  +  "</tr>"; 

  for (var i=0; i < this.cvp_bins.length; ++i)
    result += this.cvp_bins[i].stringify();

  return result;
}





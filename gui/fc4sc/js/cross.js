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

function Cross(node, cross_bins) {

  var name = node.getAttribute('name');
  var expr = '';
  var total_bins = 0;

  for (var it = 0; it < node.childNodes.length; ++it) {
    if (node.childNodes[it].nodeType == 1) {
      if (node.childNodes[it].nodeName == "ucis:crossBin")
        break;
      else if (node.childNodes[it].nodeName == "ucis:crossExpr")
        expr += " " + node.childNodes[it].firstChild.data + " x";
    }
  }

  expr = expr.slice(0,-1);

  for (var it = node.childNodes.length - 1; it >= 0; --it) {
    if (node.childNodes[it].nodeType == 1 && node.childNodes[it].nodeName == "ucis:userAttr") {
      total_bins = node.childNodes[it].getAttribute('key');
      break;
    }
  }

  var proc = (cross_bins.length * 100 / total_bins).toFixed(2);

  var option_node;
  for (var i =0; i < node.childNodes.length; ++i ) {
    if (node.childNodes[i].nodeType == 1 && node.childNodes[i].nodeName == "ucis:options") {
      option_node = node.childNodes[i];
      break;
    }
  }

  this.name = name;

  if (!this.name)
    this.name = "cross coverage";

  this.exprString = expr;
  this.cvp_bin_count = cross_bins.length + 1;
  this.cvp_procent = proc;
  this.cvp_weight = option_node.getAttribute('weight');
  this.cvp_bins = cross_bins;
  this.total_bins = total_bins;
  this.hit_bins =  cross_bins.length;
  this.style = get_proc_style(false, cross_bins.length, total_bins - cross_bins.length);

  this.cg_inst_lines = this.hit_bins + 4;

}

Cross.prototype.stringify = function() {
  var result = "";
  
  result += "<tr>" 
  +    "<td " + this.style + " rowspan=" + (this.cvp_bin_count + 2) + ">" 
  +      this.cvp_procent + "%"
  +    "</td>"
  +  "</tr>"; 


  result += 
       "<tr class=\"cvp\">"
  +       "<td colspan=\"5\">" + this.name + "</td>"
  +    "</tr>" 
  +    "<tr>"
  +      "<td class=\"cvp\" colspan=\"2\">"
  +       this.exprString
  +      "</td>"
  +      "<td class=\"cvp\" colspan=\"3\">" 
  +       String(this.hit_bins) + "/" + String(this.total_bins) + " bins hit" 
  +      "</td>"
  +    "</tr>"; 

  
  for (var i=0; i < this.cvp_bins.length; ++i)
    result += this.cvp_bins[i].stringify();

  return result;

}

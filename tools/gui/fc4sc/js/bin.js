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

function Bin(node) {
  var val1 = 0;
  var val2 = 0;
  var range_count = 0;

  var total_hits = parseInt(node.getAttribute('alias'));
  var name = node.getAttribute('name');
  var ranges = [];
  var range = 0;

  this.type = String(node.getAttribute('type'));

  for (var index = 0; index < node.childNodes.length; ++index) {
    if (node.childNodes[index].nodeType == 1 && node.childNodes[index].nodeName == "ucis:range" ) {

      range++;

      val1 = node.childNodes[index].getAttribute("from");
      val2 = node.childNodes[index].getAttribute("to");

      if (val1 == val2) {
        ranges.push(val1.toString());
      } else {
        ranges.push("[" + val1.toString() + ":" + val2.toString() + "]");
      }

      if (total_hits > 0) 
        pass_bin++;
      else 
        fail_bin++;

    }
  }

  this.rowspan = ranges.length
  this.name = name;
  this.ranges = ranges;
  this.total_hits = total_hits / this.ranges.length;

  this.style_class = this.get_style_class();
  this.tick = this.get_tick();
}

Bin.prototype.get_style_class = function() {

  if (!this.type.localeCompare('illegal'))
    return "bin_" + (this.total_hits == 0 ? "illegal" : "fail");

  return "bin_" + ((this.total_hits > 0) ? "pass" : "fail");

}

Bin.prototype.get_tick = function() {

  if (!this.type.localeCompare('illegal'))
    return (this.total_hits != 0 ? "&#10008;" : "&#10004;")

  return (this.total_hits == 0 ? "&#10008;" : "&#10004;" )
}

Bin.prototype.get_style_class = function() {

  if (!this.type.localeCompare('illegal'))
    return "bin_" + (this.total_hits == 0 ? "illegal" : "fail");

  return "bin_" + ((this.total_hits > 0) ? "pass" : "fail");

}

Bin.prototype.get_tick = function() {

  if (!this.type.localeCompare('illegal'))
    return (this.total_hits != 0 ? "&#10008;" : "&#10004;")

  return (this.total_hits == 0 ? "&#10008;" : "&#10004;" )
}

Bin.prototype.stringify = function() {
 var result = "";
 var rowspan = this.ranges.length.toString();

 result  += "<tr  class=\""+ this.style_class +"\">" 
 +    "<td class=\"bin_header\" rowspan=" + rowspan + ">" + this.name + "</td>"
 +    "<td>" + this.ranges[0] + "</td>"
 +    "<td rowspan=" + rowspan + ">" + this.total_hits + "</td>"
 +    "<td rowspan=" + rowspan + ">" + this.tick + "</td>"
 +  "</tr>";


 for (var i = 1; i < this.ranges.length; i++) {
  result  += "<tr class=\"" + this.style_class +"\">"  
  +    "<td>" + this.ranges[i] + "</td>"
  +  "</tr>";
}

return result;
};

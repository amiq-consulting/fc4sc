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

function CrossBin(node) {
  this.indexes = [];
  this.count = 0;

  for (var index = 0; index < node.childNodes.length; ++index) {
    if (node.childNodes[index].nodeType == 1) {

      if (node.childNodes[index].nodeName == "ucis:index")
        this.indexes.push(node.childNodes[index].firstChild.data);
      else if (node.childNodes[index].nodeName == "ucis:contents") {
        this.count = node.childNodes[index].getAttribute("coverageCount");
        break;
      }
    }

  }
}

CrossBin.prototype.stringify = function() {
  var result = "";

      result  += "<tr  class=\"bin_" + ((this.count == 0) ? "fail" : "pass") +"\">" 
      +    "<td colspan=\"2\">" + "{" + this.indexes + "}" + "</td>"
      +    "<td>" + this.count + "</td>" 
      +    "<td>" + (this.count == 0 ? "&#10008;" : "&#10004;" ) + "</td>"
      +  "</tr>";

      return result;
};








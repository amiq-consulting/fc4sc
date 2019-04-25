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

function CgType(node, cvg_tree) {
    if (typeof node === "string" || node instanceof String) this.name = node;
    else this.name = node.getAttribute("moduleName");

    this.cg_insts = cvg_tree;
}

CgType.prototype.stringify = function() {
    var result = "";

    result +=
        '<div style="text-align: center;" id="' +
        this.name +
        '">' +
        "<h1>" +
        this.name +
        "</h1>";

    // elem.innerHTML += result;

    for (var k = 0; k < this.cg_insts.length; ++k) {
        result += this.cg_insts[k].stringify();
    }

    // elem.innerHTML += "</div>";
    result += "</div>";

    console.log(result);

    return result;
};

CgType.prototype.toc_stringify = function() {
    return "<li>" + '<a href="#' + this.name + '">' + this.name + "</a></li>";
};

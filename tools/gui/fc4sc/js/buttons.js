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

function button_change(index) {
    if (typeof button_change.state == "undefined") {
        button_change.state = [true, true, true];
    }

    if (global_cvg_type.length == 0) {
        alert("No data loaded!");
        document.getElementById("button1").checked = false;
        document.getElementById("button2").checked = false;
        document.getElementById("button3").checked = false;
        return;
    }

    index = index - 1;

    button_change.state[index] = !button_change.state[index];

    var printed = [];
    element1.innerHTML = "";

    var toc = document.getElementById("toc_header");

    toc.innerHTML = "<br><h4>Covergroup types</h4><ul>";

    toc = document.getElementById("toc");
    toc.innerHTML = "";

    for (var it1 = 0; it1 < global_cvg_type.length; ++it1) {
        var type = global_cvg_type[it1];

        var insts_printed = [];

        for (var it2 = 0; it2 < type.cg_insts.length; ++it2) {
            var inst = type.cg_insts[it2];

            var cvps = [];
            var procs = [];
            var cvp_weights = [];
            var count = 0;
            for (var it3 = 0; it3 < inst.cvg_cvps.length; ++it3) {
                var cvp = inst.cvg_cvps[it3];

                if (
                    (button_change.state[0] == true &&
                        cvp.cvp_procent == 100) ||
                    (button_change.state[2] == true &&
                        cvp.cvp_procent == 0.0) ||
                    (button_change.state[1] == true &&
                        cvp.cvp_procent < 100 &&
                        cvp.cvp_procent > 0)
                ) {
                    cvps.push(cvp);
                    procs.push(cvp.cvp_procent);
                    cvp_weights.push(cvp.cvp_weight);
                    count += cvp.cg_inst_lines;
                }
            }

            if (cvps.length != 0) {
                insts_printed.push(
                    new Cg(procs, cvp_weights, cvps, count, {
                        name: inst.name,
                        style: inst.style
                    })
                );
            }
        }

        if (insts_printed.length != 0) {
            var print_type = new CgType(String(type.name), insts_printed);
            element1.innerHTML += print_type.stringify();
            toc.innerHTML += print_type.toc_stringify();
        }
    }

    toc.innerHTML += '<li><a href="#top">Back to top </a></li>';
    toc.innerHTML += "</ul>";
    toc.innerHTML += "<br>";
}

function print_options(node) {
    var name = node.attributes;
    var result = "<table>";

    for (var it = 0; it < node.attributes.length; ++it) {
        result += "<tr>";
        result += "<td>" + node.attributes[it].name + "</td>";
        result += "<td>    </td>";
        result += '<td>"' + node.attributes[it].value + '"</td>';
        result += "</tr>";
    }
    return result + "</table>";
}

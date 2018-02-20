function Cg(cvp_procs, cvp_weights, global_cvp_tree, cg_inst_lines, choice) {

  this.cvg_proc = 0;
  this.w_sum = 0;

  for (var i=0; i< cvp_procs.length; ++i) {
    this.cvg_proc += cvp_procs[i] * cvp_weights[i];
    this.w_sum += Number(cvp_weights[i]);
  }

  this.cvg_proc = this.cvg_proc / this.w_sum;       
  this.cvg_proc = this.cvg_proc.toFixed(2);


  if ('node' in choice) {
    var node = choice['node'];
    this.name = node.getAttribute('name');  

    this.style = "proc_partial";
    if (node.getAttribute("excluded") == "true")
      this.style = "proc_excluded";
    else if (this.cvg_proc == 0.00) 
      this.style = "proc_empty";
    else if (this.cvg_proc == 100.00)
      this.style = "proc_full";  

  } else {

    this.name = choice['name'];
    this.style = choice['style'];

  }
  
  this.cg_inst_lines = cg_inst_lines + 1;

  this.cvg_cvps = new Array(global_cvp_tree.length);
  for (var i=0; i < global_cvp_tree.length; ++i)
    this.cvg_cvps[i] = global_cvp_tree[i];

  this.cvg_procs = cvp_procs;
  this.cvp_weights = cvp_weights;

  return this;

}

Cg.prototype.stringify = function() {

  result = "<table id=\"cg_table\" border=\"1\"> " 
  +    "<tr>" 
  +      "<th class=\"proc " + this.style + "\" rowspan=\"" + this.cg_inst_lines +"\">" 
  +        String(this.cvg_proc) + "%" 
  +      "</th>"
  +      "<th class=\"cg_inst\" colspan=\"5\">" 
  +        this.name 
  +      "</th>"
  +    "</tr>"; 


  for (var i=0; i < this.cvg_cvps.length; ++i)
    result += this.cvg_cvps[i].stringify();

  result += "</table>";
  result += "<br>";

  return result;

};


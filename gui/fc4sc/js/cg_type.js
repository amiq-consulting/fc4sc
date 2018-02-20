
function CgType(node, cvg_tree) {

  if (typeof node === 'string' || node instanceof String)
    this.name = node;
  else
    this.name = node.getAttribute('moduleName');

  this.cg_insts = cvg_tree;
  
}

CgType.prototype.stringify = function(elem) {

  var result = "";

  result += "<div style=\"text-align: center;\" id=\"" + this.name + "\">" 
  +    "<h1>"
  +      this.name
  +    "</h1>";

  elem.innerHTML += result;

  for (var k=0; k < this.cg_insts.length; ++k ) {
      elem.innerHTML += this.cg_insts[k].stringify();
  }

  elem.innerHTML += "</div>";

  return result;

};

CgType.prototype.toc_stringify = function() {
  return "<li>" 
        +  "<a href=\"#" + this.name + "\">"
        +    this.name 
        +  "</a></li>";  

}
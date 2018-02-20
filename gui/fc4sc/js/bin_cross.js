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








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
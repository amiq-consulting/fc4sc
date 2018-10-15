"""
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

               Date: 2018-Oct-07
******************************************************************************/
"""
import os
import sys
import xml.etree.ElementTree as ET
from fnmatch import fnmatch

def find_xmls(directory):
    for rootdir, _, files in os.walk(directory):
        for fname in files:
            if fnmatch(fname, '*.xml'):
                filename = os.path.join(rootdir, fname)
                yield filename

def parseXML(name):
    tree = ET.parse(name)
    root = tree.getroot()
    
    if root.tag != "{UCIS}UCIS":
        print("Skipping non-UCIS XML")
        return
    else:
        print("Root has tag [{0}] and attribute [{1}]".format(root.tag, root.attrib))
    
    for child in root:
        if child.attrib:
            print (child.tag, child.attrib)
        else:
            print (child.tag)
    
    
    # namespace map
    nsstr = 'ucis'
    ns = {nsstr : 'UCIS'}
    
    # parse module hierarchy (instanceCoverages elements)
    for instanceCoverages in root.findall('{0}:instanceCoverages'.format(nsstr), ns):
        print("Covergroup type: {0}; {1}".format(instanceCoverages.get('moduleName'), instanceCoverages.attrib))
        # parse covergroups (covergroupCoverage elements)
        for covergroupCoverage in instanceCoverages.findall('{0}:covergroupCoverage'.format(nsstr), ns):
            # parse covergroup instances (cgInstance elements)
            for cgInstance in covergroupCoverage.findall('{0}:cgInstance'.format(nsstr), ns):
                print ("\t[cgInstance] {0}".format(cgInstance.get('name')))
                # parse coverpoints
                for coverpoint in cgInstance.findall('{0}:coverpoint'.format(nsstr), ns):
                    print ("\t\t[coverpoint] {0}".format(coverpoint.get('name')))
                    # parse bins
                    for bin in coverpoint.findall('{0}:coverpointBin'.format(nsstr), ns):
                        totalhits = 0
                        # parse all bin ranges
                        for range in bin.findall('{0}:range'.format(nsstr), ns):
                            contents = range.find('{0}:contents'.format(nsstr), ns)
                            rangeHitCount = int(contents.get('coverageCount'))
                            totalhits = totalhits + rangeHitCount
                            
                        print ("\t\t\t[bin:{1}] {0} -> {2}".format(
                            bin.get('name'), bin.get('type'), totalhits))

if __name__ == "__main__":
    # the search top directory is by default the execution directory 
    search_top_dir = os.getcwd()
    merged_db_name = "coverage_merged_db.xml"
    if len(sys.argv) > 1: # if specified file path
        search_top_dir = sys.argv[1]
    if len(sys.argv) > 2: # if specified merged database name
        merged_db_name = sys.argv[2]

    print("Searching for XMLs in top directory: \n" + search_top_dir)
    merged_db_path = os.path.join(search_top_dir, merged_db_name)
    
    for filename in find_xmls(search_top_dir):
        print ('*****Parsing XML: ' + filename)
        parseXML(filename)
    
    # TODO: parse the resulted DB and change the "UCIS ID" attributes to be unique
    
    

    print("Done!")
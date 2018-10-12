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
    print("Parsing XML...")
    print("Root has tag [{0}] and attribute [{1}]".format(root.tag, root.attrib))
    
    for child in root:
        print child.tag, child.attrib

if __name__ == "__main__":
    # the search top directory is by default the execution directory 
    search_top_dir = os.getcwd()
    if len(sys.argv) > 1: # if specified file path
        search_top_dir = sys.argv[1]

    print("Top directory: " + search_top_dir)
    merged_db_path = os.path.join(search_top_dir, "coverage_merged_db.xml")
    
    for filename in find_xmls(search_top_dir):
        print ('Found XML source: ' + filename)
        
        parseXML(filename)
               

    print("Done!")
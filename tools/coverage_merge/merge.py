import os
import sys
from fnmatch import fnmatch

def find_xmls(directory):
    for rootdir, _, files in os.walk(directory):
        for fname in files:
            if fnmatch(fname, '*.xml'):
                filename = os.path.join(rootdir, fname)
                yield filename

if __name__ == "__main__":
    # the search top directory is by default the execution directory 
    search_top_dir = os.getcwd()
    if len(sys.argv) > 1: # if specified file path
        search_top_dir = sys.argv[1]

    print("Top directory: " + search_top_dir)
    merged_db_path = os.path.join(search_top_dir, "coverage_merged_db.xml")
    
    for filename in find_xmls(search_top_dir):
        print ('Found XML source: ' + filename)
        # TODO: parse xml file and produce a merged coverage db
        # also check that the file is using UCIS schema       

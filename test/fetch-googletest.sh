#!/bin/bash

# This script can be used to download and extract the googletest framework

GOOGLETEST_DIR="googletest"

if [ -d $GOOGLETEST_DIR ]; then
	echo "Googletest directory already exists! Nothing to do!"
	echo "If you need a clean download, remove the googletest directory and run the script again!"
	exit 0
fi
# download googletest framework version 1.8.0
wget https://github.com/google/googletest/archive/release-1.8.0.tar.gz &&
# extract & remove the archive
tar xf release-1.8.0.tar.gz && rm release-1.8.0.tar.gz &&
# rename the top directory to plain "googletest"
mv googletest-release-1.8.0 $GOOGLETEST_DIR && 
echo "Googletest framework download completed successfully!" &&
echo "Top directory: "`pwd`"/"$GOOGLETEST_DIR









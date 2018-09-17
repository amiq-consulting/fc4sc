#!/bin/bash
#******************************************************************************#
#   Copyright 2018 AMIQ Consulting s.r.l.
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
#
#******************************************************************************#
#   Original Authors: Teodor Vasilache and Dragos Dospinescu,
#                     AMIQ Consulting s.r.l. (contributors@amiq.com)
#
#               Date: 2018-Feb-20
#******************************************************************************#

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









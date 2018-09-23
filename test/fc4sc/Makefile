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

# in order to compile with debug flags, use: 
# make DEBUGFLAGS="-ggdb -g3"

SRC_DIR := .
OBJ_DIR := ./obj
GOOGLE_TEST_HOME := ./../googletest/
INCLUDE_FC4SC := -I./../../includes
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

LDFLAGS := -lpthread -L $(GOOGLE_TEST_HOME)/build/googlemock/gtest -lgtest
CPPFLAGS := -I $(GOOGLE_TEST_HOME)/googletest/include $(INCLUDE_FC4SC)
CXXFLAGS := -std=c++11 -Wextra $(DEBUGFLAGS)

CC := g++
EXEC := main.exe

.PHONY: dir clean

$(EXEC): $(OBJ_FILES)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | dir
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $< -MMD

dir:
	mkdir -p obj

clean:
	rm -rf $(OBJ_DIR) *.xml  $(EXEC)

run:	
	export LD_LIBRARY_PATH=$(GOOGLE_TEST_HOME)/build/googlemock/gtest && ./$(EXEC)

-include $(OBJ_DIR)/*.d

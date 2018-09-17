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

CC = g++
LD = g++

EXEC = fir

INCLUDE_SYSTEMC = -I${SYSTEMC_HOME}/include
INCLUDE_FC4SC = -I./../../includes

INCLUDES = $(INCLUDE_SYSTEMC) $(INCLUDE_FC4SC) 
CFLAGS = -c -std=c++11
DEFINES = -DFC4SC_NO_THROW
LDFLAGS = -L${SYSTEMC_HOME}/lib-linux64 -lsystemc 
LDPATH = export LD_LIBRARY_PATH=${SYSTEMC_HOME}/lib-linux64
SRCFILES = $(wildcard src/*.cpp)

OBJFILES = $(patsubst %.cpp, obj/%.o, ${SRCFILES})

_dummy := $(shell mkdir -p obj/src)

all: main

main: $(OBJFILES)
	$(LD) $(LDFLAGS) $(OBJFILES) -o $(EXEC)

obj/%.o: %.cpp
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) $^ -o $@

run:
	$(LDPATH) && ./$(EXEC)

clean:
	rm -rf obj $(EXEC)

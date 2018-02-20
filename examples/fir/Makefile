CC = g++
LD = g++

EXEC = fir

INCLUDES = -I./../../includes -I${SYSTEMC_HOME}/include
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

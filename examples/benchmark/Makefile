CC = g++
LD = g++

EXEC = test

INCLUDES = -I./../../includes
CFLAGS = -std=c++11
DEFINES = -DFC4SC_NO_THROW
LDFLAGS = 
LDPATH = true

SRCFILES = $(wildcard *.cpp)

OBJFILES = $(patsubst %.cpp, obj/%.o, ${SRCFILES})

_dummy := $(shell mkdir -p obj/src)

all: main

main: $(OBJFILES)
	$(LD) $(LDFLAGS) $(OBJFILES) -o $(EXEC)

obj/%.o: %.cpp
	$(CC) -c $(CFLAGS) $(DEFINES) $(INCLUDES) $^ -o $@

run:
	$(LDPATH) && ./$(EXEC)

clean:
	rm -rf obj/ $(EXEC)

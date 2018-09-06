SRC_DIR := .
OBJ_DIR := ./obj
GOOGLE_TEST_HOME := ./../googletest/
INCLUDE_FC4SC := -I./../../includes
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
LDFLAGS := -lpthread -L $(GOOGLE_TEST_HOME)/build/googlemock/gtest -lgtest
CPPFLAGS := -I $(GOOGLE_TEST_HOME)/googletest/include $(INCLUDE_FC4SC)
CXXFLAGS := -std=c++11 -Wall
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

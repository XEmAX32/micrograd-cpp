CXX = g++
CXXFLAGS = -Wall -O2 -Isrc -I/opt/homebrew/include/graphviz
LDFLAGS = -L/opt/homebrew/lib -lgvc -lcgraph -lcdt
DEPFLAGS = -MMD -MP

SRCDIR = src
BUILD_DIR = build
TARGET = $(BUILD_DIR)/main

SRC_MAIN = main.cpp
SRC_SUB  = $(wildcard $(SRCDIR)/*.cpp)
SRC      = $(SRC_MAIN) $(SRC_SUB)

OBJ_MAIN = $(BUILD_DIR)/main.o
OBJ_SUB  = $(SRC_SUB:$(SRCDIR)/%.cpp=$(BUILD_DIR)/%.o)
OBJ      = $(OBJ_MAIN) $(OBJ_SUB)

DEP_MAIN = $(OBJ_MAIN:.o=.d)
DEP_SUB  = $(OBJ_SUB:.o=.d)
DEP      = $(DEP_MAIN) $(DEP_SUB)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ) | $(BUILD_DIR)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

# rule for main.cpp → build/main.o
$(BUILD_DIR)/main.o: main.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

# rule for src/foo.cpp → build/foo.o
$(BUILD_DIR)/%.o: $(SRCDIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

-include $(DEP)
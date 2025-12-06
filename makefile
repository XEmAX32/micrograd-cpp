CXX = g++
CXXFLAGS = -Wall -O2 -I/opt/homebrew/include/graphviz
LDFLAGS = -L/opt/homebrew/lib -lgvc -lcgraph -lcdt

BUILD_DIR = build
TARGET = $(BUILD_DIR)/graph
OBJ = $(BUILD_DIR)/graph.o $(BUILD_DIR)/value.o

all: $(TARGET)

# make sure build/ exists before linking
$(TARGET): $(OBJ) | $(BUILD_DIR)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

# pattern rule for object files inside build/
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# rule to create the build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
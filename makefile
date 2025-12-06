CXX = g++
CXXFLAGS = -Wall -O2 -I/opt/homebrew/include/graphviz
LDFLAGS = -L/opt/homebrew/lib -lgvc -lcgraph -lcdt

BUILD_DIR = build
TARGET = $(BUILD_DIR)/graph
OBJ = $(BUILD_DIR)/graph.o

all: $(TARGET)

$(TARGET): $(OBJ) | $(BUILD_DIR)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
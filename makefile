CXX = g++
CXXFLAGS = -Wall -O2 -I/opt/homebrew/include/graphviz
LDFLAGS = -L/opt/homebrew/lib -lgvc -lcgraph -lcdt

TARGET = graph
OBJ = graph.o value.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)
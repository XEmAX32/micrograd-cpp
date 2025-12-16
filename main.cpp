#include <iostream>
#include <vector>
#include <algorithm>
#include "src/value.h"
#include "src/nn.h"
#include "src/graph.h"

int main() {
  // binary classifier

  MLP n(3, std::vector<int>{4, 4, 1});

  std::vector<Value*> parameters = n.parameters();

  for (auto& parameter : parameters) 
    std::cout << parameter->getPointer()->data << std::endl;

  std::cout << "size: " << parameters.size() << std::endl;

  const std::vector<std::vector<Value>> data{
    {2.0, 3.0, -1.0},
    {3.0, -1.0, 0.5},
    {0.5, 1.0, 1.0},
    {1.0, 1.0, -1.0}
  };

  std::vector<std::vector<Value>> expected{{1.0}, {-1.0}, {-1.0}, {1.0}};
  n.train(data, expected, 50);

  return 0;
}
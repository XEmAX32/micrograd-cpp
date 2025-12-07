#include <iostream>
#include "value.h"
#include "graph.h"

int main() {

  // inputs x1, x2
  Value x1(2.0, "x1");
  Value x2(0.0, "x2");

  // weights w1, w2
  Value w1(-3.0, "w1");
  Value w2(1.0, "w2");

  // bias of the neuron
  Value b(6.7, "b");

  Value x1w1 = x1 * w1; x1w1.label = "x1*w1";
  Value x2w2 = x2 * w2; x2w2.label = "x2*w2";
  Value x1w1x2w2 = x1w1 + x2w2; x1w1x2w2.label = "x1*w1x2*w2";
  Value n = x1w1x2w2 + b; n.label = "n";

  // for (const Value* child_node : v5.prev) {
  //   std::cout << *child_node << std::endl;
  // }

  // std::cout << v5.op << std::endl;

  render_graph(n);

  return 0;
}
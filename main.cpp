#include <iostream>
#include <vector>
#include <algorithm>
#include "src/value.h"
#include "src/graph.h"

int main() {
  Value a(-2.0, "a"); 
  Value b(3.0, "b");
  Value c = b + a; c->label = "ciao";
  Value f = a/b; f->label ="f";



  std::cout << a << " and " << f << std::endl;

  // Value d = a * b; d.label = "d";
  // Value e = a + b; e.label = "e";
  // Value f = d * e; f.label = "d";


  return 0;
}

// this is a neuron
// int main() {

//   // inputs x1, x2
//   Value x1(2.0, "x1");
//   Value x2(0.0, "x2");

//   // weights w1, w2
//   Value w1(-3, "w1");
//   Value w2(1.0, "w2");

//   // bias of the neuron
//   Value b(6.8813735870195432, "b");

//   // computations
//   Value x1w1 = x1 * w1; x1w1.label = "x1*w1";
//   Value x2w2 = x2 * w2; x2w2.label = "x2*w2";
//   Value x1w1x2w2 = x1w1 + x2w2; x1w1x2w2.label = "x1*w1x2*w2";
//   Value n = x1w1x2w2 + b; n.label = "n";

//   Value o = n.tanh(); o.label = "o";

//   // /* begin backprop */
//   // o.grad = 1.0;
//   // // computing derivative do/dn (derivative of output with respect to n)
//   // n.grad = 1 - o.data * o.data; 
//   // x1w1x2w2.grad = n.grad;
//   // b.grad = n.grad;
//   // x1w1.grad = n.grad;
//   // x2w2.grad = n.grad;

//   // x2.grad = w2.data * x2w2.grad;
//   // w2.grad = x2.data * x2w2.grad;
//   // x1.grad = w1.data * x1w1.grad;
//   // w1.grad = x1.data * x1w1.grad;

//   // /* end backprop */
 


//   // for (const Value* child_node : v5.prev) {
//   //   std::cout << *child_node << std::endl;
//   // }

//   // std::cout << v5.op << std::endl;

//   o.backprop();

//   render_graph(o);

//   return 0;
// }
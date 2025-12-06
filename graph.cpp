#include <iostream>
#include "value.h"

int main() {

  Value v1(2.00);
  Value v2(3.00);

  std::cout << "v1=" << v1 * v2 << std::endl;

    // // 1. Create a Graphviz context
    // GVC_t *gvc = gvContext();

    // // 2. Create a new graph
    // Agraph_t *g = agopen(const_cast<char*>("G"), Agdirected, 0);

    // // 3. Create nodes
    // Agnode_t *A = agnode(g, const_cast<char*>("A"), 1);
    // Agnode_t *B = agnode(g, const_cast<char*>("B"), 1);

    // // 4. Create an edge
    // agedge(g, A, B, 0, 1);

    // // 5. Layout using the DOT engine
    // gvLayout(gvc, g, "dot");

    // // 6. Render to PNG
    // gvRenderFilename(gvc, g, "png", "output.png");

    // // 7. Cleanup
    // gvFreeLayout(gvc, g);
    // agclose(g);
    // gvFreeContext(gvc);

    return 0;
}
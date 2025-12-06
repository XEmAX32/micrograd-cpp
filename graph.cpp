#include <iostream>
#include <sstream>
#include <gvc.h>
#include "value.h"


void render_node(Agraph_t* graph, const Value* current_node_value, Agnode_t* parent_node) {
  std::ostringstream ss;
  ss << *current_node_value;
  char* current_node_value_name = strdup(ss.str().c_str());

  Agnode_t* new_node = agnode(graph, current_node_value_name, 1);
  agsafeset(new_node, (char*)"shape", (char*)"box", (char*)"");

  Agnode_t* operation_node = nullptr;

  if(current_node_value->op) {
    char buf[2] = { current_node_value->op, '\0' };
    operation_node = agnode(graph, strdup(buf), 1);
    agedge(graph, operation_node, new_node, 0, 1);
  }

  if (parent_node) {
    agedge(graph, new_node, parent_node, 0, 1);
  }

  for (const Value* child_node : current_node_value->prev) {
    render_node(graph, child_node, operation_node ? operation_node : new_node);
  }

}

void render_graph(const Value& root_node) {
    GVC_t* gvc = gvContext();
    Agraph_t* graph = agopen(const_cast<char*>("GRAPH"), Agdirected, nullptr);

    render_node(graph, &root_node, nullptr);

    if (gvLayout(gvc, graph, "dot") != 0) {
        std::cerr << "gvLayout failed\n";
    } else {
        gvRenderFilename(gvc, graph, "png", "output.png");
    }

    gvFreeLayout(gvc, graph);
    agclose(graph);
    gvFreeContext(gvc);
}

int main() {

  Value v1(2.00);
  Value v2(3.00);
  Value v3 = v1 + v2;
  Value v4(4.00);
  Value v5 = v3 * v4;

  for (const Value* child_node : v5.prev) {
    std::cout << *child_node << std::endl;
  }

  std::cout << v5.op << std::endl;

  render_graph(v5);

  return 0;
}
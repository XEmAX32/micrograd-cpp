#include <iostream>
#include <sstream>
#include <gvc.h>
#include "value.h"
#include "graph.h"

static void render_node(Agraph_t* graph, const Value* current_node_value, Agnode_t* parent_node) {
  std::ostringstream ss;
  ss << current_node_value;
  char* current_node_value_name = strdup(ss.str().c_str());

  float test = 0.02;

  Agnode_t* new_node = agnode(graph, current_node_value_name, 1);

  std::ostringstream label_ss;
  label_ss << "{ " << current_node_value->label
          << " | value: "  << *current_node_value
          // << " | grad: "  << test
          << " }";
  char* label = strdup(label_ss.str().c_str());

  agsafeset(new_node, (char*)"shape", (char*)"record", (char*)"");
  agsafeset(new_node, (char*)"label", label, (char*)"");

  Agnode_t* operation_node = nullptr;

  if(current_node_value->op) {
    char buf[10] = { current_node_value->op, '\0' };
    // make this node truly unique
    snprintf(buf, sizeof(buf), "%c%s", current_node_value->op, current_node_value->label.c_str());
    operation_node = agnode(graph, strdup(buf), 1);

    char operation_buffer[2] = { current_node_value->op, '\0' };
    agsafeset(operation_node, (char*)"label", operation_buffer, (char*)"");

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

    agsafeset(graph, (char*)"rankdir", (char*)"LR", (char*)"");

    if (gvLayout(gvc, graph, "dot") != 0) {
        std::cerr << "gvLayout failed\n";
    } else {
        gvRenderFilename(gvc, graph, "png", "output.png");
    }
    

    gvFreeLayout(gvc, graph);
    agclose(graph);
    gvFreeContext(gvc);
}
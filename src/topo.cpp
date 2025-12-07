#include <vector>
#include <set>
#include "topo.h"
#include "value.h"


static void build_topological_order_step(std::vector<const Value*>& topo, std::set<const Value*>& visited, const Value* v) {
  if (visited.count(v)) return;
  visited.insert(v);

  for (const Value* child : v->prev)
    build_topological_order_step(topo, visited, child);

  topo.push_back(v);
}

std::vector<const Value*> build_topological_order(const Value& v) {
  std::vector<const Value*> topo;
  std::set<const Value*> visited;

  build_topological_order_step(topo, visited, &v);

  return topo;
}
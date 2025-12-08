#include <vector>
#include <set>
#include "topo.h"
#include "value.h"


static void build_topological_order_step(std::vector<std::shared_ptr<ValueData>>& topo, std::set<std::shared_ptr<ValueData>>& visited, const std::shared_ptr<ValueData>& node) {
  if (visited.count(node)) return;
  visited.insert(node);

  for (const auto& child : node->prev)
    build_topological_order_step(topo, visited, child);

  topo.push_back(node);
}

std::vector<std::shared_ptr<ValueData>> build_topological_order(const std::shared_ptr<ValueData>& root) {
  std::vector<std::shared_ptr<ValueData>> topo;
  std::set<std::shared_ptr<ValueData>> visited;

  build_topological_order_step(topo, visited, root);

  return topo;
}
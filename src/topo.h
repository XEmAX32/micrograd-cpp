#pragma once 

#include <vector>

struct ValueData;

std::vector<std::shared_ptr<ValueData>> build_topological_order(const std::shared_ptr<ValueData>& root);

#pragma once 

#include <vector>

class Value;

std::vector<const Value*> build_topological_order(const Value& v);

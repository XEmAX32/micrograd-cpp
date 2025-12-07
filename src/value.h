#pragma once

#include <iostream>
#include <set>
#include <string>
#include <cmath>
#include <functional>
#include "topo.h"

class Value {
  public: 
    float data; 
    std::string label;
    std::set<const Value*> prev; 
    std::function<void()> backward;
    std::string op; 
    float grad; 

    Value(float d = 0,
          std::string lab = "",
          std::set<const Value*> children = {},
          std::string operation = "",
          float grad = 0.00
        )
        : data(d), label(lab), prev(children), backward([]() {}), op(operation), grad(grad) {}

    friend std::ostream& operator<<(std::ostream& os, const Value& v) {
        os << v.data;
        return os;
    }

    Value operator+(Value& other) {
        Value out(data + other.data, "", { this, &other }, "+", 0);
        out.backward = [this, &out, &other]() {
            // using += so that repeatedly used node will accumulate gradient
            // same applies in other backward functions, not going to repeat comment
            this->grad += 1.0 * out.grad;
            other.grad += 1.0 * out.grad;
        };

        return out;
    }

    Value operator*(Value& other) {
        Value out(data * other.data, "", { this, &other }, "*", 0);

        out.backward = [this, &out, &other]() {
            this->grad += other.data * out.grad;
            other.grad += this->data * out.grad;
        };

        return out;
    }

    Value tanh() {
        float result = (std::exp(2 * data) - 1) / (std::exp(2 * data) + 1);
        Value out(result, "", { this }, "tanh", 0);

        out.backward = [this, &out, result]() {
            this->grad += (1 - result * result) * out.grad;
        };

        return out;
    }

    void backprop() {
        this->grad = 1.0;

        std::vector<const Value*> ordered_values = build_topological_order(*this);
        std::reverse(ordered_values.begin(), ordered_values.end());
      
        for (const Value* node : ordered_values)
          const_cast<Value*>(node)->backward();
    }
};

#pragma once

#include <iostream>
#include <set>
#include <string>

class Value {
  public: 
    float data; 
    std::string label;
    std::set<const Value*> prev; 
    char op; 

    Value(float d = 0,
          std::string lab = "",
          std::set<const Value*> children = {},
          char operation = '\0')
        : data(d), label(lab), prev(children), op(operation) {}

    friend std::ostream& operator<<(std::ostream& os, const Value& v) {
        os << v.data;
        return os;
    }

    Value operator+(const Value& other) const {
        return Value(data + other.data, "\0", { this, &other }, '+');
    }

    Value operator*(const Value& other) const {
        return Value(data * other.data, "\0", { this, &other }, '*');
    }
};

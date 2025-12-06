#ifndef VALUE_H
#define VALUE_H

#include <iostream>
#include <set>

class Value {
  public: 
    float data; 
    std::set<const Value*> prev; 
    char op; 

    Value(float d = 0,
          std::set<const Value*> children = {},
          char operation = '\0')
        : data(d), prev(children), op(operation) {}

    friend std::ostream& operator<<(std::ostream& os, const Value& v) {
        os << v.data;
        return os;
    }

    Value operator+(const Value& other) const {
        return Value(data + other.data, { this, &other }, '+');
    }

    Value operator*(const Value& other) const {
        return Value(data * other.data, { this, &other }, '*');
    }
};

#endif
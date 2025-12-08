#pragma once

#include <memory>
#include <type_traits>
#include <iostream>
#include <set>
#include <string>
#include <cmath>
#include <functional>
#include "topo.h"

struct ValueData {
    float data;
    std::string label;
    std::set<std::shared_ptr<ValueData>> prev;
    std::string op;
    float grad = 0.0;
    std::function<void()> backward = []() {};

    ValueData(float data,
              std::string label = "",
              std::set<std::shared_ptr<ValueData>> prev = {},
              std::string op = "",
              float grad = 0.0)
        : data(data), label(std::move(label)), prev(std::move(prev)), op(std::move(op)), grad(grad) {}
};

class Value {
    private:
        std::shared_ptr<ValueData> internal_pointer;

    public: 
        Value(double data, std::string label = "")
            : internal_pointer(std::make_shared<ValueData>(data, label)) {}

        explicit Value(std::shared_ptr<ValueData> n) : internal_pointer(std::move(n)) {}

        friend std::ostream& operator<<(std::ostream& os, const Value& v) {
            os << v.internal_pointer->data;
            return os;
        }

        ValueData* operator->() const {
            return internal_pointer.get();
        }
    
        std::shared_ptr<ValueData> getPointer() const {
            return internal_pointer;
        }

        /* begin addition operator implementation */
        Value operator+(Value other) {
            auto out = std::make_shared<ValueData>(
                internal_pointer->data + other.internal_pointer->data,
                "",
                std::set<std::shared_ptr<ValueData>>{ internal_pointer, other.internal_pointer },
                "+",
                0.0
            );

            out->backward = [this, other, out]() {
                // using += so that repeatedly used node will accumulate gradient
                // same applies in other backward functions, not going to repeat comment
                this->internal_pointer->grad += 1.0 * out->grad;
                other.internal_pointer->grad += 1.0 * out->grad;
            };

            return Value(out);
        }

        template <typename T>
            requires std::is_arithmetic_v<T>
        Value operator+(T other) {
            return add_value_number(*this, other);
        }

        template <typename T>
            requires std::is_arithmetic_v<T>
        friend Value operator+(T c, Value& v) {
            return add_value_number(v, c);
        }
        /* end addition operator implementation */

        /* begin multiplication operator implementation */
        Value operator*(Value other) {
            auto out = std::make_shared<ValueData>(
                internal_pointer->data * other.internal_pointer->data,
                "",
                std::set<std::shared_ptr<ValueData>>{ internal_pointer, other.internal_pointer },
                "*",
                0.0
            );

            out->backward = [this, other, out]() {
                // using += so that repeatedly used node will accumulate gradient
                // same applies in other backward functions, not going to repeat comment
                this->internal_pointer->grad += other.internal_pointer->data * out->grad;
                other.internal_pointer->grad += this->internal_pointer->grad * out->grad;
            };

            return Value(out);
        }

        template <typename T>
            requires std::is_arithmetic_v<T>
        Value operator*(T other) {
            return multiply_value_number(*this, other);
        }

        template <typename T>
            requires std::is_arithmetic_v<T>
        friend Value operator*(T c, Value& v) {
            return multiply_value_number(v, c);
        }
        /* end multiplicaton operator implementation */

        Value tanh() {
            float result = (std::exp(2 * this->internal_pointer->data) - 1) / (std::exp(2 * this->internal_pointer->data) + 1);

            auto out = std::make_shared<ValueData>(
                result,
                "",
                std::set<std::shared_ptr<ValueData>>{ internal_pointer },
                "tanh",
                0.0
            );

            out->backward = [this, result, out]() {
                this->internal_pointer->grad += (1 - result * result) * out->grad;
            };

            return Value(out);
        }

        void backprop() {
            this->internal_pointer->grad = 1.0;

            auto topo = build_topological_order(internal_pointer);
            std::reverse(topo.begin(), topo.end());
            
            for (auto& node : topo)
                node->backward();
        }

    private:
        template <typename T>
            requires std::is_arithmetic_v<T>
        static Value add_value_number(Value& v, T other) {
            auto other_pointer = std::make_shared<ValueData>(
                other,
                ""
            );
            auto out = std::make_shared<ValueData>(
                other_pointer->data + v->data,
                "",
                std::set<std::shared_ptr<ValueData>>{ v.internal_pointer, other_pointer },
                "+",
                0.0
            );

            out->backward = [v, other_pointer, out]() {
                // using += so that repeatedly used node will accumulate gradient
                // same applies in other backward functions, not going to repeat comment
                v->grad += 1.0 * out->grad;
                other_pointer->grad += 1.0 * out->grad;
            };

            return Value(out);
        }

        template <typename T>
            requires std::is_arithmetic_v<T>
        static Value multiply_value_number(Value& v, T other) {
            auto other_pointer = std::make_shared<ValueData>(
                other,
                ""
            );
            auto out = std::make_shared<ValueData>(
                other_pointer->data * v->data,
                "",
                std::set<std::shared_ptr<ValueData>>{ v.internal_pointer, other_pointer },
                "*",
                0.0
            );

            out->backward = [v, other_pointer, out]() {
                // using += so that repeatedly used node will accumulate gradient
                // same applies in other backward functions, not going to repeat comment
                v->grad += other_pointer->data * out->grad;
                other_pointer->grad += v->data * out->grad;
            };

            return Value(out);
        }
};

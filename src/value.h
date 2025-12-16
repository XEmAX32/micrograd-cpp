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
    double data;
    std::string label;
    std::set<std::shared_ptr<ValueData>> prev;
    std::string op;
    double grad = 0.0;
    std::function<void()> backward = []() {};

    ValueData(double data,
              std::string label = "",
              std::set<std::shared_ptr<ValueData>> prev = {},
              std::string op = "",
              double grad = 0.0)
        : data(data), label(std::move(label)), prev(std::move(prev)), op(std::move(op)), grad(grad) {}
};

class Value {
    private:
        std::shared_ptr<ValueData> internal_pointer;

    public: 
        Value(double data, std::string label = "", std::set<std::shared_ptr<ValueData>> prev = {}, std::string op = "", double grad = 0.0)
            : internal_pointer(std::make_shared<ValueData>(data, label, prev, op, grad)) {}

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

            out->backward = [a = internal_pointer, other, out]() {
                // using += so that repeatedly used node will accumulate gradient
                // same applies in other backward functions, not going to repeat comment
                a->grad += 1.0 * out->grad;
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

        Value& operator+=(const Value& other) {
            *this = *this + other;
            return *this;
        }

        template <typename T>
            requires std::is_arithmetic_v<T>
        Value& operator+=(const T other) {
            *this = *this + other;
            return *this;
        }
        /* end addition operator implementation */

        /* begin subtraction operator implementation */
        Value operator-() {
            return (*this) * -1;
        }

        Value operator-(Value other) {
            return (*this) + (-other);
        }

        template <typename T>
            requires std::is_arithmetic_v<T>
        Value operator-(T other) {
            return (*this) + (-other);
        }

        template <typename T>
            requires std::is_arithmetic_v<T>
        friend Value operator-(T other, Value val) {
            return -val + other;
        }
        /* end subtraction operator implementation */

        /* begin multiplication operator implementation */
        Value operator*(Value other) {
            auto out = std::make_shared<ValueData>(
                internal_pointer->data * other.internal_pointer->data,
                "",
                std::set<std::shared_ptr<ValueData>>{ internal_pointer, other.internal_pointer },
                "*",
                0.0
            );

            out->backward = [a = internal_pointer, b = other.internal_pointer, out]() {
                // using += so that repeatedly used node will accumulate gradient
                // same applies in other backward functions, not going to repeat comment
                a->grad += b->data * out->grad;
                b->grad += a->data * out->grad;
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

        /* begin division operator implementation */

        Value operator/(Value other) {
            return (*this) * other.pow(-1);
        }

        template <typename T>
            requires std::is_arithmetic_v<T>
        Value operator/(T other) {
            return (*this) * std::pow(other, -1);
        }

        template <typename T>
            requires std::is_arithmetic_v<T>
        friend Value operator/(T other, Value val) {
            return val.pow(-1) * other;
        }

        /* end division operator implementation */

        // Value operator/() {
        //     return *(this)->pow(-1);
        // }

        // Value operator/(Value other) {
        //     auto out = std::make_shared<ValueData>(
        //         internal_pointer->data / other.internal_pointer->data,
        //         "",
        //         std::set<std::shared_ptr<ValueData>>{ internal_pointer, other.internal_pointer },
        //         "/",
        //         0.0
        //     );

        //     out->backward = [this, other, out]() {
        //         // using += so that repeatedly used node will accumulate gradient
        //         // same applies in other backward functions, not going to repeat comment
        //         this->internal_pointer->grad += 1 / other.internal_pointer->data * out->grad;
        //         other.internal_pointer->grad += this->internal_pointer->grad * out->grad;
        //     };

        //     return Value(out);
        // }

        template <typename T>
            requires std::is_arithmetic_v<T>
        Value pow(T exponent) {
            auto out = std::make_shared<ValueData>(
                std::pow(this->internal_pointer->data, exponent),
                "",
                std::set<std::shared_ptr<ValueData>>{ internal_pointer },
                "pow",
                0.0
            );

            out->backward = [a = internal_pointer, out, exponent]() {
                a->grad += exponent * (std::pow(a->data, exponent - 1)) * out->grad;
            };

            return Value(out);
        }

        Value exp() {
            auto out = std::make_shared<ValueData>(
                std::exp(this->internal_pointer->data),
                "",
                std::set<std::shared_ptr<ValueData>>{ internal_pointer },
                "exp",
                0.0
            );

            out->backward = [a = internal_pointer, out]() {
                a->grad += out->data * out->grad;
            };

            return Value(out);
        }

        Value tanh() {
            Value intermediate = (2 * (*this)).exp();
            Value result = (intermediate - 1) / (intermediate + 1);

            return result;
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

#include <vector>
#include <random>
#include <numeric>
#include <stdexcept>
#include "value.h"

class Neuron {
  private:
    std::vector<Value> weights;
    Value bias;
  public: 
    Neuron(int nin) : weights(), bias(0.0) {
      // init weights
      weights.reserve(nin);

      // create and persist one random generator
      static std::random_device rd;
      static std::mt19937 gen(rd());
      static std::uniform_real_distribution<> dist(-1.0, 1.0);

      // AAA: check here why this and not emplace_back
      for (int i = 0; i < nin; i++) weights.emplace_back(dist(gen));
      bias = dist(gen);
    }

    Value operator()(std::vector<Value> weights_to_multiply, double acc_start = 0.0) {
      Value sum(acc_start);

      // we should add errors if weights_to_multiply isnt compatible with weights for matrix multiplication
      for (size_t i = 0; i < weights.size(); i++)
        sum += weights[i] * weights_to_multiply[i];

      // pre-activate
      sum += bias;

      // return activated value
      return sum.tanh();
    }

    Value& operator[](size_t i) {
        return weights[i];
    }

    std::vector<Value*> parameters() {
      std::vector<Value*> out;
      out.reserve(1 + weights.size());

      out.push_back(&bias);
      for(auto& weight : weights) out.push_back(&weight);

      return out;
    }

};

class Layer {
  std::vector<Neuron> neurons;

  public:
    Layer(int nin, int nout) : neurons() {
      neurons.reserve(nout);

      for(int i = 0; i < nout; i++) neurons.emplace_back(nin);
    }

    std::vector<Value> operator()(std::vector<Value> weights_to_multiply) {
      std::vector<Value> outs;
      outs.reserve(neurons.size());

      for(Neuron& neuron : neurons) outs.emplace_back(neuron(weights_to_multiply));

      return outs;
    }

    std::vector<Value*> parameters() {
      std::vector<Value*> out;

      for (Neuron& neuron : neurons) {
        std::vector<Value*> temp = neuron.parameters();
        out.insert(out.end(), temp.begin(), temp.end());
      };

      return out;
    }

    size_t size() const {
      return neurons.size();
    }
};

class MLP {
  std::vector<Layer> layers;
  public: 
    MLP(int nin, const std::vector<int>& nouts) : layers() {
      std::vector<int> sizes = { nin };
      sizes.insert(sizes.end(), nouts.begin(), nouts.end());

      layers.reserve(nouts.size());

      for(size_t i = 0; i < nouts.size(); ++i) layers.emplace_back(sizes[i], sizes[i + 1]);
    }

    std::vector<Value> operator()(std::vector<Value> weights_to_multiply) {
      for (Layer& layer : layers) weights_to_multiply = layer(weights_to_multiply);

      return weights_to_multiply;
    }

    std::vector<Value*> parameters() {
      std::vector<Value*> out;

      for (Layer& layer : layers) {
        std::vector<Value*> temp = layer.parameters();
        out.insert(out.end(), temp.begin(), temp.end());
      };

      return out;
    }

    void train(std::vector<std::vector<Value>> data, std::vector<std::vector<Value>> expected, const int steps = 10) {
      std::vector<std::vector<Value>> predicted;
      Value loss = 0.0;
      std::vector<Value*> parameters = this->parameters();

      // const size_t output_layer_size = layers[layers.size() - 1].size();
      // const size_t input_layer_size = layers[0].size();

      // for (std::vector<Value> expected_row : expected) {
      //   if (expected_row.size() != output_layer_size)
      //     throw std::runtime_error("Expected vector has wrong size (expected it to be: " 
      //         + std::to_string(output_layer_size) + ")");
      // }

      // for (std::vector<Value> data_row : data) {
      //   if (data_row.size() != input_layer_size)
      //     throw std::runtime_error("Data vector has wrong size (expected it to be: " 
      //         + std::to_string(input_layer_size) + ", got: " + std::to_string(data_row.size()) + " )");
      // }

      for (int i = 0; i < steps; i++) {
        // zero grad + cleanup
        for (auto parameter : parameters) parameter->getPointer()->grad = 0.0;
        loss = 0.0;
        predicted.clear();

        // forward pass
        for (std::vector<Value> data_element : data) predicted.push_back(this->operator()(data_element));
        for (size_t m = 0; m < expected.size(); m++) {
          for (size_t n = 0; n < expected[0].size(); n++) {
            Value diff = predicted[m][n] - expected[m][n];
            loss += diff * diff;
          }
        }

        // backward pass
        loss.backprop();
        parameters = this->parameters();

        // update
        for (auto parameter : parameters) {
          parameter->getPointer()->data += -0.1 * parameter->getPointer()->grad;
        }
        
        std::cout << "step: " << i + 1 << " loss: " << loss->data << std::endl; 
      }
    }
};
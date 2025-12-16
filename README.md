# micrograd-cpp

Tiny C++11 port of Karpathy’s micrograd: a minimal autograd engine plus a small MLP demo.

## Features
- Scalar `Value` objects with reverse-mode autodiff and a tiny computation-graph builder.
- Basic ops: add, sub, mul, div (via pow), pow, exp, tanh; each carries its own backward.
- A simple multi-layer perceptron (`MLP`) with random init and in-place SGD updates.
- Optional graph rendering of the computation via Graphviz (dot) to `output.png`.

## Project Layout
- `main.cpp` — example binary classifier training loop.
- `src/value.h` — core autodiff scalar.
- `src/topo.cpp/.h` — topological sort for backprop.
- `src/nn.h` — `Neuron`, `Layer`, and `MLP` definitions.
- `src/graph.cpp/.h` — Graphviz renderer for a `Value` graph.

## Build
Requires a C++11 compiler and `make`.

```bash
make
```

If you enable graph rendering, you also need Graphviz dev libraries (provides `gvc.h` and `-lgvc`, e.g., `brew install graphviz` on macOS or `sudo apt-get install graphviz libgraphviz-dev` on Debian/Ubuntu).

## Run
The default `main.cpp` trains a small MLP on a toy binary dataset.

```bash
./main
```

You’ll see per-parameter gradients and loss per step. If graph rendering is invoked, `output.png` will be written in the repo root.

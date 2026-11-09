# Automata Simulator

A small C++ library for finite automata (DFA / NFA), written from scratch as a
learning project in modern C++.

The long-term goal is to check **opacity** properties: given an automaton with
*secret* states and partially *observable* events, can an outside observer ever
be certain that the system is in a secret state? As a case study, the project
will model a simple **timing attack** (an early-exit password comparison) using
discrete time steps.

> Status: early development. The core data structures are in place; the
> algorithms are not implemented yet.

## Design

- **States and symbols are integer ids** (`0, 1, 2, ...`), so they can be used
  directly as vector indices. Human-readable symbol names (e.g. `"a"`,
  `"tick"`) are stored separately.
- **`Automaton`** is the common base class: states, alphabet, start state and
  accepting states.
- **`DFA`** and **`NFA`** inherit from `Automaton` and only differ in how they
  store transitions:
  - `DFA`: `delta[s][a]` is a `std::optional<State>` (at most one next state).
  - `NFA`: `delta[s][a]` is a `std::set<State>`, plus a separate
    `epsilon[s]` table for ε-moves.
- Invalid ids (programming errors) throw exceptions; a missing transition is a
  normal result (`std::nullopt` / empty set).

## Project structure

```
automata_simulator/
├── include/
│   ├── Automaton.h
│   ├── DFA.h
│   └── NFA.h
└── src/
    ├── Automaton.cpp
    ├── DFA.cpp
    ├── NFA.cpp
    └── main.cpp
```

## Build

Requires a C++20 compiler (tested with g++ on Linux / WSL).

```bash
g++ -std=c++20 -Wall -Wextra -Iinclude src/*.cpp -o demo
./demo
```

## Example

A DFA accepting all words over {a, b} with an even number of `a`s:

```cpp
#include "DFA.h"

int main()
{
    automata::DFA d;

    automata::State even = d.addState();
    automata::State odd  = d.addState();
    automata::Symbol a = d.addSymbol("a");
    automata::Symbol b = d.addSymbol("b");

    d.setStart(even);
    d.addAccept(even);

    d.addTransition(even, a, odd);
    d.addTransition(even, b, even);
    d.addTransition(odd,  a, even);
    d.addTransition(odd,  b, odd);

    d.printInfo();
}
```

## TODO

### Core data structures
- [x] `Automaton` base class (states, alphabet, start, accepting states)
- [x] `DFA`: `addTransition`, `next`
- [x] `NFA`: `addTransition`, `addEpsilon`, `next`
- [ ] `printInfo` also prints transitions (virtual, overridden in DFA / NFA)

### Simulation
- [ ] `DFA::accepts(const Word&)`
- [ ] `NFA::epsilonClosure(const std::set<State>&)`
- [ ] `NFA::accepts(const Word&)`
- [ ] Convert a word given as names (e.g. `"a b a"`) to a `Word`

### Algorithms
- [ ] Subset construction: `DFA determinize(const NFA&)`
- [ ] Export to Graphviz DOT for visualization
- [ ] (later) DFA minimization (Hopcroft)

### Input / validation
- [ ] Simple text file format for automata + parser
- [ ] Analyzer: structural checks (start state exists, states/symbols valid,
      ...) returning a list of errors

### Opacity
- [ ] Mark secret states and observable / unobservable symbols
- [ ] Observer construction (subset construction over unobservable moves)
- [ ] Current-state opacity check, with a counterexample observation if not opaque

### Case study
- [ ] Timing attack: early-exit password comparison modeled with `tick` events
      (not opaque) vs. constant-time comparison (opaque)

### Engineering
- [ ] CMake build
- [ ] Unit tests (Catch2 or GoogleTest)
- [ ] GitHub Actions CI
- [ ] Sanitizers (`-fsanitize=address,undefined`) in debug builds
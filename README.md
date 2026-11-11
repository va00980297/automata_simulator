# Automata Simulator

A DFA / NFA simulator written in modern C++ (C++20).

Possible future extensions include automata analysis such as opacity checking.

## How automata are represented

The design follows the textbook definition of a finite automaton as a 5-tuple
(Q, Σ, δ, q₀, F) and the transition tables used in automata theory courses:
δ is stored as a table that can be looked up directly with `delta[state][symbol]`.

### States and symbols are integer ids

```cpp
using State  = std::size_t;
using Symbol = std::size_t;
using Word   = std::vector<Symbol>;
```

States are numbered `0 .. stateCount()-1` and symbols `0 .. symbolCount()-1`,
in the order they are added.

Why ids instead of names or `char`:

- **Direct indexing.** Ids are vector indices, so a transition lookup is a
  plain array access instead of a search in a map.
- **No limit to single characters.** Symbols can have names like `"tick"` or
  `"req"`, not just `'a'`.
- **No clash with ε.** Epsilon moves are stored in a separate table in the NFA,
  so ε never needs a reserved character in the alphabet.
- **Easy to create new states.** Algorithms such as the subset construction
  can simply number new states `0, 1, 2, ...`.

### Ids and names

Human-readable names are kept separately and only used for input and output:

```cpp
std::vector<std::string> symbolNames;   // symbol id -> name
```

- `addSymbol("a")` stores the name and returns its id.
- `symbolNames[id]` turns an id back into its name, e.g. in `printInfo()`.

Internally, every algorithm only works with ids.

### Transitions

| Class | Transition table | Meaning |
|---|---|---|
| `DFA` | `delta[s][a]` → `std::optional<State>` | at most one next state; `std::nullopt` = no transition |
| `NFA` | `delta[s][a]` → `std::set<State>` | any number of next states; empty set = no transition |
| `NFA` | `epsilon[s]` → `std::set<State>` | states reachable by one ε-move (one level less: ε reads no symbol) |

Tables grow lazily when transitions are added, so partial automata
(missing transitions) are allowed.

## Class structure and access

```
Automaton            states, alphabet, start state, accepting states
├── DFA              + deterministic transition table
└── NFA              + nondeterministic transition table + ε-moves
```

| Class | `public` | `protected` | `private` |
|---|---|---|---|
| `Automaton` | building (`addState`, `addSymbol`, `setStart`, `addAccept`), getters, `printInfo` | shared data, `checkState` / `checkSymbol` | – |
| `DFA` | `addTransition`, `next`, `accepts` | – | `delta` |
| `NFA` | `addTransition`, `addEpsilon`, `next` | – | `delta`, `epsilon` |

Why it is split this way:

- **Data is never public.** Outside code can only change an automaton through
  functions that validate their input first, so an automaton can't end up
  in an invalid state (e.g. a transition to a state that doesn't exist).
- **Shared data is `protected`.** `DFA` and `NFA` need direct access to the
  states, alphabet and the `check*` helpers, but outside code doesn't.
- **Transition tables are `private`.** Only the class that owns a table knows
  its layout.

### Errors

- An invalid id (state or symbol that doesn't exist) is a programming error
  and throws `std::out_of_range`.
- A conflicting DFA transition or a duplicate symbol name throws
  `std::invalid_argument`.
- A missing transition is a normal result: `next()` returns `std::nullopt`
  (DFA) or an empty set (NFA), and `accepts()` returns `false`.

## Project structure

```
include/      Automaton.h, DFA.h, NFA.h
src/          implementations + main.cpp (demo)
tests/        tests.cpp
```

## Build

With CMake:

```bash
cmake -S . -B build
cmake --build build
./build/demo
./build/run_tests
```

Or directly with g++:

```bash
g++ -std=c++20 -Wall -Wextra -Iinclude src/*.cpp -o demo
./demo
```

## Example

```cpp
#include "DFA.h"

// DFA accepting words over {a, b} with an even number of a's
int main()
{
    automata::DFA d;

    auto even = d.addState();
    auto odd  = d.addState();
    auto a = d.addSymbol("a");
    auto b = d.addSymbol("b");

    d.setStart(even);
    d.addAccept(even);

    d.addTransition(even, a, odd);
    d.addTransition(even, b, even);
    d.addTransition(odd,  a, even);
    d.addTransition(odd,  b, odd);

    d.printInfo();
    bool ok = d.accepts({a, b, a});   // true: two a's
}
```

## TODO

- [x] Core data structures (`Automaton`, `DFA`, `NFA`)
- [x] DFA word simulation (`accepts`)
- [x] Basic tests
- [ ] NFA ε-closure and word simulation
- [ ] Name → id lookup, so words can be given as names
- [ ] NFA to DFA conversion (subset construction)
- [ ] Graphviz export
- [ ] Maybe: opacity checking and a small timing attack case study
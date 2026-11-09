#pragma once

#include <optional>
#include <vector>

#include "Automaton.h"

namespace automata
{
    // Deterministic automaton: at most one next state per (state, symbol).
    class DFA : public Automaton // public inheritance: a DFA "is an" Automaton
    {
    public:
        // Add from --a--> to. Throws if an id is invalid
        // or if (from, a) already leads to a different state.
        void addTransition(State from, Symbol a, State to);

        // Return the next state, or std::nullopt if there is no transition.
        // Throws std::out_of_range if s or a is invalid.
        std::optional<State> next(State s, Symbol a) const;

    private:
        // delta[s][a] = next state, or std::nullopt if no transition.
        // Grown lazily, so it may be smaller than stateCount() x symbolCount().
        std::vector<std::vector<std::optional<State>>> delta;
    };
} // namespace automata
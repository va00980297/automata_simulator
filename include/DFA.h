#pragma once

#include <optional>
#include <vector>

#include "Automaton.h"

namespace automata
{
    // Deterministic: at most one next state per (state, symbol).
    // tip: public inheritance = "a DFA is an Automaton", base members keep their access
    class DFA : public Automaton
    {
    public:
        // Add from --a--> to.
        // Throws if an id is invalid or (from, a) already goes somewhere else.
        void addTransition(State from, Symbol a, State to);

        // Next state, or std::nullopt if there's no transition.
        std::optional<State> next(State s, Symbol a) const;

        // Run the word through the DFA, true if it ends in an accepting state.
        bool accepts(const Word &word) const;

    private:
        // delta[s][a] = next state, or std::nullopt.
        // Grows lazily, so it can be smaller than stateCount() x symbolCount().
        std::vector<std::vector<std::optional<State>>> delta;
    };
} // namespace automata
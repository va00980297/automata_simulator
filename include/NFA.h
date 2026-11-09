#pragma once

#include <set>
#include <vector>

#include "Automaton.h"

namespace automata
{
    // Nondeterministic automaton: any number of next states, plus epsilon moves.
    class NFA : public Automaton
    {
    public:
        // Add from --a--> to. Adding the same transition twice has no effect.
        void addTransition(State from, Symbol a, State to);

        // Add an epsilon move from --ε--> to (no symbol is read).
        void addEpsilon(State from, State to);

        // Return all states reachable from s by reading a (may be empty).
        std::set<State> next(State s, Symbol a) const;

    private:
        // delta[s][a] = list of possible next states
        std::vector<std::vector<std::set<State>>> delta;
        // epsilon[s] = states reachable from s by one epsilon move
        std::vector<std::set<State>> epsilon;
    };
} // namespace automata
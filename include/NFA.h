#pragma once

#include <set>
#include <vector>

#include "Automaton.h"

namespace automata
{
    // Nondeterministic: any number of next states, plus epsilon moves.
    class NFA : public Automaton
    {
    public:
        // Add from --a--> to. Adding the same one twice does nothing.
        void addTransition(State from, Symbol a, State to);

        // Add from --eps--> to (moves without reading a symbol).
        void addEpsilon(State from, State to);

        // All states reachable from s by reading a (may be empty).
        std::set<State> next(State s, Symbol a) const;

    private:
        // delta[s][a] = set of next states
        std::vector<std::vector<std::set<State>>> delta;
        // epsilon[s] = states reachable from s by one epsilon move.
        // One level less than delta: epsilon reads no symbol.
        std::vector<std::set<State>> epsilon;
    };
} // namespace automata
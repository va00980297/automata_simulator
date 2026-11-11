#include "NFA.h"

namespace automata
{
    void NFA::addTransition(State from, Symbol a, State to)
    {
        checkState(from);
        checkState(to);
        checkSymbol(a);

        if (delta.size() < numStates)
        {
            delta.resize(numStates);
        }

        auto &row = delta[from];
        if (row.size() < symbolCount())
        {
            row.resize(symbolCount());
        }

        row[a].insert(to);
    }

    void NFA::addEpsilon(State from, State to)
    {
        checkState(from);
        checkState(to);

        if (epsilon.size() < numStates)
        {
            epsilon.resize(numStates);
        }

        epsilon[from].insert(to);
    }

    std::set<State> NFA::next(State s, Symbol a) const
    {
        checkState(s);
        checkSymbol(a);

        // Unlike a DFA, several targets are allowed. Skip exact duplicates
        if (s >= delta.size() || a >= delta[s].size())
        {
            return {};
        }
        return delta[s][a];
    }
} // namespace automata
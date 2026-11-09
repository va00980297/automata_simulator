#include "DFA.h"

namespace automata
{
    void DFA::addTransition(State from, Symbol a, State to)
    {
        // Validate first, so nothing is changed if an id is wrong.
        checkState(from);
        checkState(to);
        checkSymbol(a);

        // operator[] never grows a vector, so make sure the table is big enough.
        if (delta.size() < numStates)
        {
            delta.resize(numStates);
        }

        auto &row = delta[from];  // reference: changes go into delta itself
        if (row.size() < symbolCount())
        {
            row.resize(symbolCount());  // new cells are std::nullopt
        }

        // Determinism: a different target for (from, a) is not allowed.
        // Short-circuit: *row[a] is only evaluated if row[a] has a value.
        if (row[a].has_value() && *row[a] != to)
        {
            throw std::invalid_argument("DFA already has a different transition here");
        }
        row[a] = to;
    }

    std::optional<State> DFA::next(State s, Symbol a) const
    {
        checkState(s);
        checkSymbol(a);

        // Outside the lazily grown table = transition never added.
        if (s >= delta.size() || a >= delta[s].size())
        {
            return std::nullopt;
        }
        return delta[s][a];
    }
}  // namespace automata
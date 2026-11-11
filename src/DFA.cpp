#include "DFA.h"

namespace automata
{
    void DFA::addTransition(State from, Symbol a, State to)
    {
        // Check everything before touching the table.
        checkState(from);
        checkState(to);
        checkSymbol(a);

        // tip: v[i] never grows a vector (only push_back/resize do),
        // writing past the end is undefined behavior
        if (delta.size() < numStates)
        {
            delta.resize(numStates);
        }

        // tip: auto& = reference. Plain auto would copy the row and our write would be lost
        auto &row = delta[from];
        if (row.size() < symbolCount())
        {
            row.resize(symbolCount()); // new cells start as std::nullopt
        }

        // Same target twice is fine, a different one breaks determinism.
        // tip: && short-circuits, so *row[a] only runs if there is a value
        if (row[a].has_value() && *row[a] != to)
        {
            throw std::invalid_argument("DFA already has a different transition here");
        }
        row[a] = to;
    }

    std::optional<State> DFA::next(State s, Symbol a) const
    {
        // Bad id = caller's bug -> exception.
        checkState(s);
        checkSymbol(a);

        // Valid id but outside the table = never added -> just "no transition".
        if (s >= delta.size() || a >= delta[s].size())
        {
            return std::nullopt;
        }
        return delta[s][a];
    }

    // tip: two consts here. const Word& = won't change the word,
    // trailing const = won't change the DFA
    bool DFA::accepts(const Word &word) const
    {
        checkState(startState); // catches "no states at all", even for the empty word

        State current = startState; // local copy, fine to change in a const function
        for (Symbol a : word)       // tip: small types by value, big ones by const&
        {
            std::optional<State> nxt = next(current, a);
            if (!nxt) // tip: an optional works as a bool, false when empty
            {
                return false; // stuck -> reject
            }
            current = nxt.value(); // .value() throws if empty, *nxt wouldn't check
        }

        return acceptStates.contains(current); // contains() is new in C++20
    }
} // namespace automata
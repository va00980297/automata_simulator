#include "Automaton.h"

#include <iostream>

namespace automata
{
    // tip: Automaton:: says "this is the member function of Automaton", not a free function
    void Automaton::printInfo() const
    {
        std::cout << "========== AUTOMATON INFO ==========\n";

        std::cout << "States: ";
        for (State s = 0; s < numStates; ++s)
        {
            std::cout << s << ' ';
        }
        std::cout << '\n';

        // Print names instead of ids, easier to read.
        // tip: const& in a range-for = no copy of each string
        std::cout << "Alphabet: ";
        for (const std::string &name : symbolNames)
        {
            std::cout << name << ' ';
        }
        std::cout << '\n';

        std::cout << "Start State: " << startState << '\n';

        std::cout << "Accept States: ";
        for (State s : acceptStates) // a set always iterates in sorted order
        {
            std::cout << s << ' ';
        }
        std::cout << '\n';
    }

    State Automaton::addState()
    {
        // tip: x++ returns the OLD value, so the first call returns 0
        return numStates++;
    }

    Symbol Automaton::addSymbol(const std::string &name)
    {
        for (const std::string &existing : symbolNames)
        {
            if (existing == name)
            {
                throw std::invalid_argument("duplicate symbol: " + name);
            }
        }
        symbolNames.push_back(name);
        return symbolNames.size() - 1; // just appended -> it's the last index
    }

    void Automaton::checkState(State s) const
    {
        // Unsigned, so s >= 0 is free. Only the upper bound needs a check.
        if (s >= numStates)
        {
            throw std::out_of_range("invalid state: " + std::to_string(s));
        }
    }

    void Automaton::checkSymbol(Symbol a) const
    {
        if (a >= symbolNames.size())
        {
            throw std::out_of_range("invalid symbol: " + std::to_string(a));
        }
    }

    void Automaton::setStart(State s)
    {
        checkState(s); // check first, then change -> object never ends up broken
        startState = s;
    }

    void Automaton::addAccept(State s)
    {
        checkState(s);
        acceptStates.insert(s); // already in there? set just ignores it
    }
} // namespace automata
#include "Automaton.h"

#include <iostream>

namespace automata
{
    void Automaton::printInfo() const
    {
        std::cout << "========== AUTOMATON INFO ==========\n";

        // States are just 0 .. numStates-1.
        std::cout << "States: ";
        for (State s = 0; s < numStates; ++s)
        {
            std::cout << s << ' ';
        }
        std::cout << '\n';

        // Print names, not ids: human-readable.
        std::cout << "Alphabet: ";
        for (const std::string &name : symbolNames)
        {
            std::cout << name << ' ';
        }
        std::cout << '\n';

        std::cout << "Start State: " << startState << '\n';

        // std::set iterates in sorted order.
        std::cout << "Accept States: ";
        for (State s : acceptStates)
        {
            std::cout << s << ' ';
        }
        std::cout << '\n';
    }

    State Automaton::addState()
    {
        // Post-increment: returns the old value (= new id), then counts up.
        return numStates++;
    }

    Symbol Automaton::addSymbol(const std::string &name)
    {
        // Names must be unique. const&: read each name without copying.
        for (const std::string &existing : symbolNames)
        {
            if (existing == name)
            {
                throw std::invalid_argument("duplicate symbol: " + name);
            }
        }
        symbolNames.push_back(name);
        // Appended at the end -> its id is the last index.
        return symbolNames.size() - 1;
    }

    void Automaton::checkState(State s) const
    {
        // State is unsigned, so only the upper bound needs checking.
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
        checkState(s); // validate before changing anything
        startState = s;
    }

    void Automaton::addAccept(State s)
    {
        checkState(s);
        acceptStates.insert(s); // inserting an existing state has no effect
    }
} // namespace automata
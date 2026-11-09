#pragma once

#include <cstddef>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

namespace automata // groups our names to avoid clashes; not a type, cannot create objects
{
    // States and symbols are consecutive ids 0, 1, 2, ...
    // -> they can be used directly as vector indices.
    using State = std::size_t;
    using Symbol = std::size_t; // ids instead of char: allows names like "tick"
    using Word = std::vector<Symbol>;

    // Base class for DFA and NFA: everything except the transitions.
    class Automaton
    {
    public:
        virtual ~Automaton() = default; // needed because this class is inherited

        void printInfo() const;

        State addState();                          // returns the new state's id
        Symbol addSymbol(const std::string &name); // const&: no copy, read-only
        void setStart(State s);
        void addAccept(State s);

        // Getters: let outside code read protected data without changing it.
        std::size_t stateCount() const { return numStates; }
        std::size_t symbolCount() const { return symbolNames.size(); }

    protected:
        // Throw std::out_of_range if the id is invalid.
        void checkState(State s) const;
        void checkSymbol(Symbol a) const;

        std::size_t numStates = 0;            // states are 0 .. numStates-1
        std::vector<std::string> symbolNames; // symbol id -> name
        State startState = 0;                 // built-in type: always initialize!
        std::set<State> acceptStates;         // starts empty; no duplicates, sorted
    };
} // namespace automata
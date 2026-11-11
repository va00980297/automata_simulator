#pragma once

#include <cstddef>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

// tip: a namespace is just a name prefix (automata::State), not a type
namespace automata
{
    // States and symbols are plain ids 0, 1, 2, ... so they double as vector indices.
    using State = std::size_t;  // tip: size_t is unsigned, so -1 can't mean "none"
    using Symbol = std::size_t; // ids instead of char -> names like "tick" work too
    using Word = std::vector<Symbol>;

    // Shared part of DFA and NFA. Only the transitions live in the subclasses.
    class Automaton
    {
    public:
        // tip: any class meant to be inherited from gets a virtual destructor
        virtual ~Automaton() = default;

        void printInfo() const;

        State addState();                          // returns the new state's id
        Symbol addSymbol(const std::string &name); // throws on duplicate names
        void setStart(State s);
        void addAccept(State s);

        // Read-only access to protected data.
        // tip: trailing const = this function doesn't modify the object
        std::size_t stateCount() const { return numStates; }
        std::size_t symbolCount() const { return symbolNames.size(); }

    protected:
        // Throw std::out_of_range for ids that don't exist.
        void checkState(State s) const;
        void checkSymbol(Symbol a) const;

        std::size_t numStates = 0;            // states are 0 .. numStates-1
        std::vector<std::string> symbolNames; // symbol id -> name, e.g. 0 -> "a"
        State startState = 0;                 // tip: built-in types start as garbage, always init
        std::set<State> acceptStates;         // set: no duplicates, kept sorted
    };
} // namespace automata
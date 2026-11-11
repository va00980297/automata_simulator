#include <iostream>
#include <stdexcept>
#include <string>

#include "DFA.h"
#include "NFA.h"

// tip: anonymous namespace = only visible inside this file
namespace
{
    int failures = 0;

    // Print PASS/FAIL and count the failures.
    void check(const std::string &name, bool condition)
    {
        if (condition)
        {
            std::cout << " [PASS] " << name << "\n";
        }
        else
        {
            std::cout << " [FAIL] " << name << "\n";
            ++failures;
        }
    }

    // ========== DFA ==========

    // Words over {a, b} with an even number of a's.
    // Ids: even = 0, odd = 1, a = 0, b = 1.
    // tip: returning an object by value is cheap, the compiler skips the copy
    automata::DFA makeDFA()
    {
        automata::DFA dfa;

        automata::State even = dfa.addState();
        automata::State odd = dfa.addState();
        automata::Symbol a = dfa.addSymbol("a");
        automata::Symbol b = dfa.addSymbol("b");

        dfa.setStart(even);
        dfa.addAccept(even);

        dfa.addTransition(even, a, odd);
        dfa.addTransition(even, b, even);
        dfa.addTransition(odd, a, even);
        dfa.addTransition(odd, b, odd);

        return dfa;
    }

    void testDfaAccepts()
    {
        automata::DFA dfa = makeDFA(); // fresh DFA per test, no leftovers
        const automata::Symbol a = 0, b = 1;

        // tip: {a, b, a} is turned into a Word automatically
        check("DFA accepts aba", dfa.accepts({a, b, a}));
        check("DFA rejects ab", !dfa.accepts({a, b}));
        check("DFA accepts bbb", dfa.accepts({b, b, b}));
        check("DFA accepts empty word", dfa.accepts({}));
    }

    void testDfaErrors()
    {
        automata::DFA dfa = makeDFA();

        // Conflicting transition must throw.
        // tip: once something throws, the rest of try is skipped and we jump to catch
        bool threw = false;
        try
        {
            dfa.addTransition(0, 0, 0); // even --a--> even, but even --a--> odd exists
        }
        // tip: catch only the type we expect, always by const&. No name needed if unused
        catch (const std::invalid_argument &)
        {
            threw = true;
        }
        check("DFA rejects conflicting transition", threw);

        // Unknown symbol in the word must throw.
        threw = false;
        try
        {
            dfa.accepts({0, 99});
        }
        catch (const std::out_of_range &)
        {
            threw = true;
        }
        check("DFA throws on invalid symbol", threw);
    }
} // namespace

int main()
{
    testDfaAccepts();
    testDfaErrors();

    std::cout << (failures == 0 ? "All tests passed.\n" : "Some tests FAILED.\n");

    // tip: exit code 0 = success, anything else = failure. ctest and CI read this
    return failures == 0 ? 0 : 1;
}
#include <iostream>

#include "DFA.h"
#include "NFA.h"

int main()
{
    automata::DFA d;
    automata::State even = d.addState();
    automata::State odd = d.addState();
    automata::Symbol a = d.addSymbol("a");
    automata::Symbol b = d.addSymbol("b");

    d.setStart(even);
    d.addAccept(even);
    d.addTransition(even, a, odd);
    d.addTransition(even, b, even);
    d.addTransition(odd, a, even);
    d.addTransition(odd, b, odd);

    d.printInfo();
    std::cout << std::boolalpha << "aba accepted: " << d.accepts({a, b, a}) << '\n';
}

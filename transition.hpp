#ifndef _TRANSITION_HPP_
#define _TRANSITION_HPP_

#include <iostream>
#include "grid.hpp"
#include "state.hpp"

struct Transition {
    enum Command {
        push, pop, add, sub, mul, div, mod, lnot, gt, ptr, sw, dup, roll, inn, inc, outn, outc,
        nop, halt
    };
    State state;
    Command command;
    int arg;
    Transition () : state(State()), command(halt) {}
    Transition (Grid pos, int dp, int cc) : state(pos, dp, cc), command(halt) {}
};
std::ostream &operator<< (std::ostream &os, const Transition::Command &command) {
    if (command == Transition::Command::push) os << "push"; else
    if (command == Transition::Command::pop)  os << "pop";  else
    if (command == Transition::Command::add)  os << "add";  else
    if (command == Transition::Command::sub)  os << "sub";  else
    if (command == Transition::Command::mul)  os << "mul";  else
    if (command == Transition::Command::div)  os << "div";  else
    if (command == Transition::Command::mod)  os << "mod";  else
    if (command == Transition::Command::lnot) os << "lnot"; else
    if (command == Transition::Command::gt)   os << "gt";   else
    if (command == Transition::Command::ptr)  os << "ptr";  else
    if (command == Transition::Command::sw)   os << "sw";   else
    if (command == Transition::Command::dup)  os << "dup";  else
    if (command == Transition::Command::roll) os << "roll"; else
    if (command == Transition::Command::inn)  os << "inn";  else
    if (command == Transition::Command::inc)  os << "inc";  else
    if (command == Transition::Command::outn) os << "outn"; else
    if (command == Transition::Command::outc) os << "outc"; else
    if (command == Transition::Command::nop)  os << "nop";  else
    if (command == Transition::Command::halt) os << "halt"; else
    os << "other";
    return os;
}

#endif

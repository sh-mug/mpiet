#ifndef _STATE_HPP_
#define _STATE_HPP_

#include "grid.hpp"

struct State {
    Grid pos;
    int dp, cc;
    State () : pos(Grid()), dp(-1), cc(-1) {}
    State (Grid pos, int dp, int cc) : pos(pos), dp(dp), cc(cc) {}
    bool operator< (State other) const {
        return pos < other.pos
            || (pos == other.pos && dp < other.dp)
            || (pos == other.pos && dp == other.dp && cc < other.cc);
    }
};

#endif

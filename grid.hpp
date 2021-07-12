#ifndef _GRID_HPP_
#define _GRID_HPP_

#include <iostream>

struct Grid {
    int x, y;
    Grid () : x(-1), y(-1) {}
    Grid (int x, int y) : x(x), y(y) {}
    Grid operator+  (const Grid &other) const { return Grid(x + other.x, y + other.y); }
    bool operator<  (const Grid &other) const { return y < other.y || (y == other.y && x < other.x); }
    bool operator== (const Grid &other) const { return y == other.y && x == other.x; }
    bool operator!= (const Grid &other) const { return !(*this == other); }
};

std::ostream &operator<< (std::ostream &os, const Grid &grid) {
    os << "(" << grid.x << " " << grid.y << ")";
    return os;
}

#endif

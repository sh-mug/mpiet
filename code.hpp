#ifndef _CODE_HPP_
#define _CODE_HPP_

#include <iostream>
#include <algorithm>
#include <array>
#include <map>
#include <stack>
#include <vector>
#include <png++/png.hpp>
#include "color.hpp"
#include "grid.hpp"
#include "state.hpp"
#include "transition.hpp"

constexpr int DP_R = 0, DP_D = 1, DP_L = 2, DP_U = 3;
constexpr int CC_L = 0, CC_R = 1;

struct Code {
    Code (png::image<png::rgb_pixel> &image, int codelsize)
      : codelsize (codelsize),
        height (image.get_height() / codelsize),
        width (image.get_width() / codelsize),
        codel_color (height, std::vector<Color>(width)),
        block_parent (height, std::vector<Grid>(width)),
        block_size (height, std::vector<int>(width, 1)),
        block_cape (height, std::vector<std::array<std::array<Grid, 2>, 4>>(width)),
        block_next (height, std::vector<std::array<std::array<Transition, 2>, 4>>(width)) {
        // set the color of each codel
        for (int y = 0; y < height; ++y)     
        for (int x = 0; x < width; ++x) {
            try {
                codel_color[y][x] = image[y * codelsize][x * codelsize];
            } catch (std::exception &err) {
                std::cerr << err.what() << " at " << Grid(x, y) << " = "
                    << image[y * codelsize][x * codelsize] << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        if (codel_color[0][0].iswhite || codel_color[0][0].isblack) {
            std::cerr << "syntax error: (0 0) = " << image[0][0] << std::endl;
            exit(EXIT_FAILURE);
        }
        // set the initial value of each block
        for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
            for (int dp = 0; dp < 4; ++dp)
            for (int cc = 0; cc < 2; ++cc)
                block_cape[y][x][dp][cc] = Grid(x, y);
        // make blocks
        for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x) {
            Color color = codel_color[y][x];
            if (color.iswhite || color.isblack) continue;
            if (y + 1 < height && color == codel_color[y + 1][x])
                block_merge(Grid(x, y), Grid(x, y + 1));
            if (x + 1 < width && color == codel_color[y][x + 1])
                block_merge(Grid(x, y), Grid(x + 1, y));
        }
        // calculate transitions
        for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x) {
            Grid pos = block_root(Grid(x, y));
            // if (pos != block_root(pos)) continue;
            if (codel_color[y][x].iswhite || codel_color[y][x].isblack) continue;
            for (int dp = 0; dp < 4; ++dp)
            for (int cc = 0; cc < 2; ++cc) {
                State now(block_cape[pos.y][pos.x][dp][cc], dp, cc);
                Transition transition;
                State &next = transition.state;
                std::map<State, bool> visited;
                bool change_cc = true;
                while (visited.find(now) == visited.end()) {
                    visited[now] = change_cc;
                    next.pos.y = now.pos.y + (now.dp == DP_D ? 1 : now.dp == DP_U ? -1 : 0);
                    next.pos.x = now.pos.x + (now.dp == DP_R ? 1 : now.dp == DP_L ? -1 : 0);
                    bool feasible =
                        0 <= next.pos.y && next.pos.y < height
                        && 0 <= next.pos.x && next.pos.x < width
                        && !codel_color[next.pos.y][next.pos.x].isblack;
                    if (feasible) {
                        change_cc = true;
                        next.pos = block_root(next.pos);
                        next.pos = block_cape[next.pos.y][next.pos.x][now.dp][now.cc];
                        next.dp = now.dp;
                        next.cc = now.cc;
                        Color &now_color = codel_color[now.pos.y][now.pos.x];
                        Color &next_color = codel_color[next.pos.y][next.pos.x];
                        if (!now_color.iswhite && !next_color.iswhite && now_color != next_color) {
                            int chg_hue = mod(next_color.hue - now_color.hue, 6);
                            int chg_light = mod(next_color.lightness - now_color.lightness, 3);
                            if (chg_hue == 0 && chg_light == 0) transition.command = transition.nop;  else
                            if (chg_hue == 0 && chg_light == 1) transition.command = transition.push; else
                            if (chg_hue == 0 && chg_light == 2) transition.command = transition.pop;  else
                            if (chg_hue == 1 && chg_light == 0) transition.command = transition.add;  else
                            if (chg_hue == 1 && chg_light == 1) transition.command = transition.sub;  else
                            if (chg_hue == 1 && chg_light == 2) transition.command = transition.mul;  else
                            if (chg_hue == 2 && chg_light == 0) transition.command = transition.div;  else
                            if (chg_hue == 2 && chg_light == 1) transition.command = transition.mod;  else
                            if (chg_hue == 2 && chg_light == 2) transition.command = transition.lnot; else
                            if (chg_hue == 3 && chg_light == 0) transition.command = transition.gt;   else
                            if (chg_hue == 3 && chg_light == 1) transition.command = transition.ptr;  else
                            if (chg_hue == 3 && chg_light == 2) transition.command = transition.sw;   else
                            if (chg_hue == 4 && chg_light == 0) transition.command = transition.dup;  else
                            if (chg_hue == 4 && chg_light == 1) transition.command = transition.roll; else
                            if (chg_hue == 4 && chg_light == 2) transition.command = transition.inn;  else
                            if (chg_hue == 5 && chg_light == 0) transition.command = transition.inc;  else
                            if (chg_hue == 5 && chg_light == 1) transition.command = transition.outn; else
                            if (chg_hue == 5 && chg_light == 2) transition.command = transition.outc; else
                            transition.command = transition.halt;
                            Grid root = block_root(now.pos);
                            transition.arg = block_size[root.y][root.x];
                            break;
                        } else {
                            now = next;
                        }
                    } else {
                        if (change_cc) now.cc = (now.cc + 1) % 2;
                        else now.dp = (now.dp + 1) % 4;
                        change_cc = change_cc ^ true;
                        now.pos = block_root(now.pos);
                        now.pos = block_cape[now.pos.y][now.pos.x][now.dp][now.cc];
                    }
                }
                // for (auto [state, use] : visited) {
                State state(Grid(x, y), dp, cc);
                bool use = true;
                {
                    if (!use) continue;
                    block_next[state.pos.y][state.pos.x][state.dp][state.cc] = transition;
                }
            }
        }
    }
    int execute (void) {
        static constexpr int SIZE = 4194304;
        std::vector<int> stack(SIZE);
        std::array<int, 1024> buf;
        int stacksize = 0, maxsize = SIZE;
        Grid startpos = block_root(Grid(0, 0));
        startpos = block_cape[startpos.y][startpos.x][DP_R][CC_L];
        Transition transition(startpos, DP_R, CC_L);
        Grid &pos = transition.state.pos;
        int &dp = transition.state.dp, &cc = transition.state.cc;
        do {
            if (stacksize == maxsize)
                stack.resize((maxsize += SIZE));
            transition = block_next[pos.y][pos.x][dp][cc];
            if (transition.command == transition.push) {
                stack[stacksize++] = transition.arg;
            } else if (transition.command == transition.pop) {
                if (stacksize < 1) goto ignored;
                stacksize--;
            } else if (transition.command == transition.add) {
                if (stacksize < 2) goto ignored;
                int b = stack[--stacksize];
                stack[stacksize - 1] += b;
            } else if (transition.command == transition.sub) {
                if (stacksize < 2) goto ignored;
                int b = stack[--stacksize];
                stack[stacksize - 1] -= b;
            } else if (transition.command == transition.mul) {
                if (stacksize < 2) goto ignored;
                int b = stack[--stacksize];
                stack[stacksize - 1] *= b;
            } else if (transition.command == transition.div) {
                if (stacksize < 2) goto ignored;
                int b = stack[--stacksize];
                if (b == 0) {
                    stacksize++;
                    goto ignored;
                }
                int a = stack[--stacksize];
                stack[stacksize++] = div(a, b);
            } else if (transition.command == transition.mod) {
                if (stacksize < 2) goto ignored;
                int b = stack[--stacksize];
                if (b == 0) {
                    stacksize++;
                    goto ignored;
                }
                int a = stack[--stacksize];
                stack[stacksize++] = mod(a, b);
            } else if (transition.command == transition.lnot) {
                if (stacksize < 1) goto ignored;
                stack[stacksize - 1] = !stack[stacksize - 1];
            } else if (transition.command == transition.gt) {
                if (stacksize < 2) goto ignored;
                int b = stack[--stacksize];
                int a = stack[--stacksize];
                stack[stacksize++] = (a > b);
            } else if (transition.command == transition.ptr) {
                if (stacksize < 1) goto ignored;
                int a = stack[--stacksize];
                dp = (dp + a) % 4;
            } else if (transition.command == transition.sw) {
                if (stacksize < 1) goto ignored;
                int a = stack[--stacksize];
                cc = (cc + a) % 2;
            } else if (transition.command == transition.dup) {
                stack[stacksize] = stack[stacksize - 1];
                stacksize++;
            } else if (transition.command == transition.roll) {
                if (stacksize < 2) goto ignored;
                int b = stack[--stacksize];
                int a = stack[--stacksize];
                if (a < 0 || b < 0 || stacksize < a) {
                    stacksize += 2;
                    goto ignored;
                }
                for (int i = 0; i < a; ++i) {
                    buf[i] = stack[--stacksize];
                }
                for (int i = 0; i < a; ++i) {
                    stack[stacksize++] = buf[mod(b - 1 - i, a)];
                }
            } else if (transition.command == transition.inn) {
                int a;
                std::cin >> a;
                stack[stacksize++] = a;
            } else if (transition.command == transition.inc) {
                char a;
                std::cin >> a;
                stack[stacksize++] = (int)a;
            } else if (transition.command == transition.outn) {
                if (stacksize < 1) goto ignored;
                std::cout << stack[--stacksize] << std::flush;
            } else if (transition.command == transition.outc) {
                if (stacksize < 1) goto ignored;
                std::cout << (char)stack[--stacksize] << std::flush;
            }
            ignored:;
        } while (transition.command != transition.halt);
        return EXIT_SUCCESS;
    }

    private:
        int codelsize, height, width;
        std::vector<std::vector<Color>> codel_color;
        std::vector<std::vector<Grid>> block_parent;
        std::vector<std::vector<int>> block_size;
        std::vector<std::vector<std::array<std::array<Grid, 2>, 4>>> block_cape;
        std::vector<std::vector<std::array<std::array<Transition, 2>, 4>>> block_next;
        Grid block_root (Grid codel) {
            Grid ret;
            if (block_parent[codel.y][codel.x] == Grid()) return codel;
            return block_parent[codel.y][codel.x] = block_root(block_parent[codel.y][codel.x]);
        }
        void block_merge (Grid a, Grid b) {
            Grid root_a = block_root(a), root_b = block_root(b);
            if (root_a == root_b)
                return;
            if (block_size[root_a.y][root_a.x] < block_size[root_b.y][root_b.x])
                std::swap(root_a, root_b);
            block_size[root_a.y][root_a.x] += block_size[root_b.y][root_b.x];
            block_parent[root_b.y][root_b.x] = root_a;
            if (block_cape[root_b.y][root_b.x][DP_R][CC_L].x > block_cape[root_a.y][root_a.x][DP_R][CC_L].x) {
                block_cape[root_a.y][root_a.x][DP_R][CC_L]   = block_cape[root_b.y][root_b.x][DP_R][CC_L];
                block_cape[root_a.y][root_a.x][DP_R][CC_R]   = block_cape[root_b.y][root_b.x][DP_R][CC_R];
            } else if (block_cape[root_b.y][root_b.x][DP_R][CC_L].x == block_cape[root_a.y][root_a.x][DP_R][CC_L].x) {
                if (block_cape[root_b.y][root_b.x][DP_R][CC_L].y < block_cape[root_a.y][root_a.x][DP_R][CC_L].y)
                    block_cape[root_a.y][root_a.x][DP_R][CC_L]   = block_cape[root_b.y][root_b.x][DP_R][CC_L];
                if (block_cape[root_b.y][root_b.x][DP_R][CC_R].y > block_cape[root_a.y][root_a.x][DP_R][CC_R].y)
                    block_cape[root_a.y][root_a.x][DP_R][CC_R]   = block_cape[root_b.y][root_b.x][DP_R][CC_R];
            }
            if (block_cape[root_b.y][root_b.x][DP_D][CC_L].y > block_cape[root_a.y][root_a.x][DP_D][CC_L].y) {
                block_cape[root_a.y][root_a.x][DP_D][CC_L]   = block_cape[root_b.y][root_b.x][DP_D][CC_L];
                block_cape[root_a.y][root_a.x][DP_D][CC_R]   = block_cape[root_b.y][root_b.x][DP_D][CC_R];
            } else if (block_cape[root_b.y][root_b.x][DP_D][CC_L].y == block_cape[root_a.y][root_a.x][DP_D][CC_L].y) {
                if (block_cape[root_b.y][root_b.x][DP_D][CC_L].x > block_cape[root_a.y][root_a.x][DP_D][CC_L].x)
                    block_cape[root_a.y][root_a.x][DP_D][CC_L]   = block_cape[root_b.y][root_b.x][DP_D][CC_L];
                if (block_cape[root_b.y][root_b.x][DP_D][CC_R].x < block_cape[root_a.y][root_a.x][DP_D][CC_R].x)
                    block_cape[root_a.y][root_a.x][DP_D][CC_R]   = block_cape[root_b.y][root_b.x][DP_D][CC_R];
            }
            if (block_cape[root_b.y][root_b.x][DP_L][CC_L].x < block_cape[root_a.y][root_a.x][DP_L][CC_L].x) {
                block_cape[root_a.y][root_a.x][DP_L][CC_L]   = block_cape[root_b.y][root_b.x][DP_L][CC_L];
                block_cape[root_a.y][root_a.x][DP_L][CC_R]   = block_cape[root_b.y][root_b.x][DP_L][CC_R];
            } else if (block_cape[root_b.y][root_b.x][DP_L][CC_L].x == block_cape[root_a.y][root_a.x][DP_L][CC_L].x) {
                if (block_cape[root_b.y][root_b.x][DP_L][CC_L].y > block_cape[root_a.y][root_a.x][DP_L][CC_L].y)
                    block_cape[root_a.y][root_a.x][DP_L][CC_L]   = block_cape[root_b.y][root_b.x][DP_L][CC_L];
                if (block_cape[root_b.y][root_b.x][DP_L][CC_R].y < block_cape[root_a.y][root_a.x][DP_L][CC_R].y)
                    block_cape[root_a.y][root_a.x][DP_L][CC_R]   = block_cape[root_b.y][root_b.x][DP_L][CC_R];
            }
            if (block_cape[root_b.y][root_b.x][DP_U][CC_L].y < block_cape[root_a.y][root_a.x][DP_U][CC_L].y) {
                block_cape[root_a.y][root_a.x][DP_U][CC_L]   = block_cape[root_b.y][root_b.x][DP_U][CC_L];
                block_cape[root_a.y][root_a.x][DP_U][CC_R]   = block_cape[root_b.y][root_b.x][DP_U][CC_R];
            } else if (block_cape[root_b.y][root_b.x][DP_U][CC_L].y == block_cape[root_a.y][root_a.x][DP_U][CC_L].y) {
                if (block_cape[root_b.y][root_b.x][DP_U][CC_L].x < block_cape[root_a.y][root_a.x][DP_U][CC_L].x)
                    block_cape[root_a.y][root_a.x][DP_U][CC_L]   = block_cape[root_b.y][root_b.x][DP_U][CC_L];
                if (block_cape[root_b.y][root_b.x][DP_U][CC_R].x > block_cape[root_a.y][root_a.x][DP_U][CC_R].x)
                    block_cape[root_a.y][root_a.x][DP_U][CC_R]   = block_cape[root_b.y][root_b.x][DP_U][CC_R];
            }
        }
        int div (int a, int b) {
            if (a % b == 0 || a >= 0) return a / b;
            else return (a / b) - 1;
        }
        int mod (int a, int b) {
            if (a % b == 0 || a >= 0) return a % b;
            else return (a % b) + b;
        }
};

#endif

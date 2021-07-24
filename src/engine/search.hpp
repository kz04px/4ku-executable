#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <chess/move.hpp>
#include <climits>
#include "time.hpp"

namespace chess {
class Position;
}  // namespace chess

namespace search {

constexpr int inf = INT_MAX;
constexpr int mate_score = inf - 1;

struct StackData {
    chess::Move pv;
};

int search(const chess::Position &pos, int alpha, const int beta, int depth, StackData *ss, const timepoint stop_time);

}  // namespace search

#endif

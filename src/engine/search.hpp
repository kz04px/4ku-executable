#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <climits>
#include "time.hpp"

namespace chess {
class Position;
class Move;
}  // namespace chess

namespace search {

constexpr int inf = INT_MAX;
constexpr int mate_score = inf - 1;

int search(const chess::Position &pos,
           int alpha,
           const int beta,
           const int depth,
           chess::Move &pv,
           const timepoint stop_time);

}  // namespace search

#endif

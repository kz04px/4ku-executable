#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <chess/move.hpp>

#define MATE_SCORE 10'000'000
#define INF 20'000'000

namespace chess {
class Position;
//class Move;
}  // namespace chess

namespace search {

struct SeachStack {
    chess::Move pv;
    chess::Move killer;
};

int alphabeta(const chess::Position &pos,
              int alpha,
              const int beta,
              int depth,
              const int ply,
              const long long int stop_time,
              SeachStack *pvline);

}  // namespace search

#endif

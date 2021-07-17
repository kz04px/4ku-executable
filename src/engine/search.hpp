#ifndef SEARCH_HPP
#define SEARCH_HPP

#define MATE_SCORE 10'000'000
#define INF 20'000'000

namespace chess {
class Position;
class Move;
}  // namespace chess

namespace search {

int alphabeta(const chess::Position &pos,
              int alpha,
              const int beta,
              int depth,
              int ply,
              const int stop_time,
              chess::Move pvline[]);

}  // namespace search

#endif

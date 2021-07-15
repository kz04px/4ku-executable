#ifndef SEARCH_HPP
#define SEARCH_HPP

#define MATE_SCORE 10'000'000
#define INF 20'000'000

namespace chess {
class Position;
class Move;
}  // namespace chess

namespace search {

int negamax(const chess::Position &pos, int alpha, const int beta, int depth, chess::Move &pv, const int stop_time);

}  // namespace search

#endif

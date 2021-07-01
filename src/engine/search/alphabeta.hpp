#ifndef SEARCH_ALPHABETA_HPP
#define SEARCH_ALPHABETA_HPP

namespace chess {
struct Position;
struct Move;
}  // namespace chess

namespace search {

[[nodiscard]] int alphabeta(chess::Position &pos, int alpha, int beta, int depth, chess::Move &pv);

}  // namespace search

#endif

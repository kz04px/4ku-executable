#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <chrono>

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
              const int ply,
              const std::chrono::time_point<std::chrono::steady_clock> stop_time,
              chess::Move *pvline);

}  // namespace search

#endif

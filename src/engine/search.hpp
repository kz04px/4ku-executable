#ifndef SEARCH_HPP
#define SEARCH_HPP

#define MATE_SCORE (1 << 15)
#define INF (1 << 16)

namespace chess {
class Position;
class Move;
}  // namespace chess

namespace search {

int alphabeta(chess::Position &pos,
              int alpha,
              const int beta,
              int depth,
              const int ply,
              const long long int stop_time,
              chess::Move *pvline);

}  // namespace search

#endif

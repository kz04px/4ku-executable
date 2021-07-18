#ifndef MOVEGEN_HPP
#define MOVEGEN_HPP

namespace chess {

struct Position;
struct Move;

void movegen(const Position &pos, Move *movelist, int &num_moves, const bool captures);

}  // namespace chess

#endif

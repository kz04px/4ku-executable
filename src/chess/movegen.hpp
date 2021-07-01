#ifndef MOVEGEN_HPP
#define MOVEGEN_HPP

namespace chess {

struct Position;
struct Move;

[[nodiscard]] int movegen(const Position &pos, Move *movelist);

}  // namespace chess

#endif

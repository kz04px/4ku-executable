#ifndef CHESS_MAKEMOVE_HPP
#define CHESS_MAKEMOVE_HPP

namespace chess {

struct Position;
struct Move;

void makemove(Position &pos, const Move &move);

}  // namespace chess

#endif

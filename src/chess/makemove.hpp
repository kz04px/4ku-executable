#ifndef CHESS_MAKEMOVE_HPP
#define CHESS_MAKEMOVE_HPP

namespace chess {

struct Position;
struct Move;

bool makemove(Position &pos, const Move &move);

}  // namespace chess

#endif

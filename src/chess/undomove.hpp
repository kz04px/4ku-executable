#ifndef CHESS_UNDOMOVE_HPP
#define CHESS_UNDOMOVE_HPP

namespace chess {

struct Position;
struct Move;

void undomove(Position &pos, const Move &move);

}  // namespace chess

#endif

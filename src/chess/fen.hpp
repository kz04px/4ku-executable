#ifndef CHESS_FEN_HPP
#define CHESS_FEN_HPP

#include <string>

namespace chess {

struct Position;

void set_fen(Position &pos, const std::string &fen);
std::string get_fen(const Position &pos);

}  // namespace chess

#endif

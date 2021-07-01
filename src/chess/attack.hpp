#ifndef CHESS_ATTACK_HPP
#define CHESS_ATAXX_HPP

namespace chess {

struct Position;

[[nodiscard]] bool attacked(const Position &pos, const int sq, const bool them);

}  // namespace chess

#endif

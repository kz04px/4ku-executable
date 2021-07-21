#ifndef MOVE_HPP
#define MOVE_HPP

#include <cstdint>
#include "piece.hpp"
#include "position.hpp"

namespace chess {

struct Move {
    int from;
    int to;
    Piece place_piece;
};

[[nodiscard]] inline bool operator==(const Move &lhs, const Move &rhs) noexcept {
    return lhs.from == rhs.from && lhs.to == rhs.to && lhs.place_piece == rhs.place_piece;
}

[[maybe_unused]] static void move_str(const Position &pos, const Move &move, char *str) {
    static constexpr char promos[] = {'\0', 'n', 'b', 'r', 'q'};

    str[0] = static_cast<char>((move.from % 8) + 'a');
    str[2] = static_cast<char>((move.to % 8) + 'a');
    if (pos.flipped) {
        str[1] = static_cast<char>(7 - (move.from / 8) + '1');
        str[3] = static_cast<char>(7 - (move.to / 8) + '1');
    } else {
        str[1] = static_cast<char>((move.from / 8) + '1');
        str[3] = static_cast<char>((move.to / 8) + '1');
    }
    str[4] = '\0';

    if (piece_on(pos, move.from) != move.place_piece) {
        str[4] = promos[static_cast<int>(move.place_piece)];
        str[5] = '\0';
    }
}

}  // namespace chess

#endif

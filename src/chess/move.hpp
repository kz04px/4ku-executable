#ifndef MOVE_HPP
#define MOVE_HPP

#include <cstdint>
#include "piece.hpp"

namespace chess {

struct Move {
    int from;
    int to;
    Piece promo;
};

[[maybe_unused]] static bool operator==(const Move &lhs, const Move &rhs) {
    return lhs.from == rhs.from && lhs.to == rhs.to && lhs.promo == rhs.promo;
}

[[maybe_unused]] static void move_str(const Move &move, char *str, const bool flip) {
    static constexpr char promos[] = {'\0', 'n', 'b', 'r', 'q', '\0', '\0'};

    str[0] = static_cast<char>((move.from % 8) + 'a');
    str[2] = static_cast<char>((move.to % 8) + 'a');
    if (flip) {
        str[1] = static_cast<char>(7 - (move.from / 8) + '1');
        str[3] = static_cast<char>(7 - (move.to / 8) + '1');
    } else {
        str[1] = static_cast<char>((move.from / 8) + '1');
        str[3] = static_cast<char>((move.to / 8) + '1');
    }

    str[4] = promos[static_cast<int>(move.promo)];
    str[5] = '\0';
}

}  // namespace chess

#endif

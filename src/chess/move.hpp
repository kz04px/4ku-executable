#ifndef MOVE_HPP
#define MOVE_HPP

#include <cstdint>
#include "piece.hpp"

namespace chess {

struct Move {
    enum class Type : std::uint8_t
    {
        Quiet = 0,
        Double,
        Capture,
        Enpassant,
        Ksc,
        Qsc,
        Promo,
        Promocapture,
    };

    Move() : type{}, piece{}, from{}, to{}, captured{}, promo{} {
    }

    Move(Type ty, Piece p, int fr, int t) : type{ty}, piece{p}, from{fr}, to{t}, captured{}, promo{} {
    }

    Move(Type ty, Piece p, int fr, int t, Piece cap) : type{ty}, piece{p}, from{fr}, to{t}, captured{cap}, promo{} {
    }

    Type type;
    Piece piece;
    int from;
    int to;
    Piece captured;
    Piece promo;
};

[[maybe_unused]] static void move_str(const Move &move, char *str, const bool flip) {
    static constexpr char promos[] = {'p', 'n', 'b', 'r', 'q', 'k'};

    str[0] = static_cast<char>((move.from % 8) + 'a');
    str[2] = static_cast<char>((move.to % 8) + 'a');
    if (flip) {
        str[1] = static_cast<char>(7 - (move.from / 8) + '1');
        str[3] = static_cast<char>(7 - (move.to / 8) + '1');
    } else {
        str[1] = static_cast<char>((move.from / 8) + '1');
        str[3] = static_cast<char>((move.to / 8) + '1');
    }
    str[4] = '\0';

    if (move.type == Move::Type::Promo || move.type == Move::Type::Promocapture) {
        str[4] = promos[static_cast<int>(move.promo)];
        str[5] = '\0';
    }
}

}  // namespace chess

#endif

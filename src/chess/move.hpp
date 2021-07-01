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

    Move(Type t, Piece p, int fr, int to) : type{t}, piece{p}, from{fr}, to{to}, captured{}, promo{} {
    }

    Move(Type t, Piece p, int fr, int to, Piece cap) : type{t}, piece{p}, from{fr}, to{to}, captured{cap}, promo{} {
    }

    Type type;
    Piece piece;
    int from;
    int to;
    Piece captured;
    Piece promo;
};

}  // namespace chess

#endif

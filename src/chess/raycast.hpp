#ifndef CHESS_RAYCAST_HPP
#define CHESS_RAYCAST_HPP

#include "bitboard.hpp"

namespace chess::raycast {

constexpr Bitboard N(const int sq, const Bitboard blockers) {
    auto mask = Bitboard(sq).north();
    for (int i = 1; i < 8; ++i) {
        mask |= (mask & ~blockers).north();
    }
    return mask;
}

constexpr Bitboard S(const int sq, const Bitboard blockers) {
    auto mask = Bitboard(sq).south();
    for (int i = 1; i < 8; ++i) {
        mask |= (mask & ~blockers).south();
    }
    return mask;
}

constexpr Bitboard E(const int sq, const Bitboard blockers) {
    auto mask = Bitboard(sq).east();
    for (int i = 1; i < 8; ++i) {
        mask |= (mask & ~blockers).east();
    }
    return mask;
}

constexpr Bitboard W(const int sq, const Bitboard blockers) {
    auto mask = Bitboard(sq).west();
    for (int i = 1; i < 8; ++i) {
        mask |= (mask & ~blockers).west();
    }
    return mask;
}

constexpr Bitboard NW(const int sq, const Bitboard blockers) {
    auto mask = Bitboard(sq).nw();
    for (int i = 1; i < 8; ++i) {
        mask |= (mask & ~blockers).nw();
    }
    return mask;
}

constexpr Bitboard NE(const int sq, const Bitboard blockers) {
    auto mask = Bitboard(sq).ne();
    for (int i = 1; i < 8; ++i) {
        mask |= (mask & ~blockers).ne();
    }
    return mask;
}

constexpr Bitboard SW(const int sq, const Bitboard blockers) {
    auto mask = Bitboard(sq).sw();
    for (int i = 1; i < 8; ++i) {
        mask |= (mask & ~blockers).sw();
    }
    return mask;
}

constexpr Bitboard SE(const int sq, const Bitboard blockers) {
    auto mask = Bitboard(sq).se();
    for (int i = 1; i < 8; ++i) {
        mask |= (mask & ~blockers).se();
    }
    return mask;
}

constexpr Bitboard knight(const int sq, const Bitboard) {
    return Bitboard(sq).knight();
}

constexpr Bitboard bishop(const int sq, const Bitboard blockers) {
    return NW(sq, blockers) | NE(sq, blockers) | SW(sq, blockers) | SE(sq, blockers);
}

constexpr Bitboard rook(const int sq, const Bitboard blockers) {
    return N(sq, blockers) | E(sq, blockers) | S(sq, blockers) | W(sq, blockers);
}

constexpr Bitboard king(const int sq, const Bitboard) {
    return Bitboard(sq).adjacent();
}

}  // namespace chess::raycast

#endif

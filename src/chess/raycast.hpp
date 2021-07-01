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
    auto mask = Bitboard(sq).north().west();
    for (int i = 1; i < 8; ++i) {
        mask |= (mask & ~blockers).north().west();
    }
    return mask;
}

constexpr Bitboard NE(const int sq, const Bitboard blockers) {
    auto mask = Bitboard(sq).north().east();
    for (int i = 1; i < 8; ++i) {
        mask |= (mask & ~blockers).north().east();
    }
    return mask;
}

constexpr Bitboard SW(const int sq, const Bitboard blockers) {
    auto mask = Bitboard(sq).south().west();
    for (int i = 1; i < 8; ++i) {
        mask |= (mask & ~blockers).south().west();
    }
    return mask;
}

constexpr Bitboard SE(const int sq, const Bitboard blockers) {
    auto mask = Bitboard(sq).south().east();
    for (int i = 1; i < 8; ++i) {
        mask |= (mask & ~blockers).south().east();
    }
    return mask;
}

constexpr Bitboard bishop(const int sq, const Bitboard blockers) {
    return NW(sq, blockers) | NE(sq, blockers) | SW(sq, blockers) | SE(sq, blockers);
}

constexpr Bitboard rook(const int sq, const Bitboard blockers) {
    return N(sq, blockers) | E(sq, blockers) | S(sq, blockers) | W(sq, blockers);
}

}  // namespace chess::raycast

#endif

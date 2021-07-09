#ifndef CHESS_RAYCAST_HPP
#define CHESS_RAYCAST_HPP

#include "bitboard.hpp"

namespace chess::raycast {

constexpr Bitboard N(const int sq, const Bitboard blockers) {
    auto mask = north(Bitboard(1ULL << sq));
    for (int i = 1; i < 8; ++i) {
        mask |= north(mask & ~blockers);
    }
    return mask;
}

constexpr Bitboard S(const int sq, const Bitboard blockers) {
    auto mask = south(Bitboard(1ULL << sq));
    for (int i = 1; i < 8; ++i) {
        mask |= south(mask & ~blockers);
    }
    return mask;
}

constexpr Bitboard E(const int sq, const Bitboard blockers) {
    auto mask = east(Bitboard(1ULL << sq));
    for (int i = 1; i < 8; ++i) {
        mask |= east(mask & ~blockers);
    }
    return mask;
}

constexpr Bitboard W(const int sq, const Bitboard blockers) {
    auto mask = west(Bitboard(1ULL << sq));
    for (int i = 1; i < 8; ++i) {
        mask |= west(mask & ~blockers);
    }
    return mask;
}

constexpr Bitboard NW(const int sq, const Bitboard blockers) {
    auto mask = nw(Bitboard(1ULL << sq));
    for (int i = 1; i < 8; ++i) {
        mask |= nw(mask & ~blockers);
    }
    return mask;
}

constexpr Bitboard NE(const int sq, const Bitboard blockers) {
    auto mask = ne(Bitboard(1ULL << sq));
    for (int i = 1; i < 8; ++i) {
        mask |= ne(mask & ~blockers);
    }
    return mask;
}

constexpr Bitboard SW(const int sq, const Bitboard blockers) {
    auto mask = sw(Bitboard(1ULL << sq));
    for (int i = 1; i < 8; ++i) {
        mask |= sw(mask & ~blockers);
    }
    return mask;
}

constexpr Bitboard SE(const int sq, const Bitboard blockers) {
    auto mask = se(Bitboard(1ULL << sq));
    for (int i = 1; i < 8; ++i) {
        mask |= se(mask & ~blockers);
    }
    return mask;
}

constexpr Bitboard knight(const int sq, const Bitboard) {
    return chess::knight(Bitboard(1ULL << sq));
}

constexpr Bitboard bishop(const int sq, const Bitboard blockers) {
    return NW(sq, blockers) | NE(sq, blockers) | SW(sq, blockers) | SE(sq, blockers);
}

constexpr Bitboard rook(const int sq, const Bitboard blockers) {
    return N(sq, blockers) | E(sq, blockers) | S(sq, blockers) | W(sq, blockers);
}

constexpr Bitboard king(const int sq, const Bitboard) {
    return adjacent(Bitboard(1ULL << sq));
}

}  // namespace chess::raycast

#endif

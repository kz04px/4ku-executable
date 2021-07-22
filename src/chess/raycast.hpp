#ifndef CHESS_RAYCAST_HPP
#define CHESS_RAYCAST_HPP

#include "bitboard.hpp"

namespace chess::raycast {

constexpr Bitboard cast(const int sq, const Bitboard blockers, Bitboard (*direction)(Bitboard)) {
    auto mask = direction(Bitboard(1ULL << sq));
    for (int i = 1; i < 8; ++i) {
        mask |= direction(mask & ~blockers);
    }
    return mask;
}

constexpr Bitboard knight(const int sq, const Bitboard) {
    return chess::knight(Bitboard(1ULL << sq));
}

constexpr Bitboard bishop(const int sq, const Bitboard blockers) {
    return cast(sq, blockers, ne) | cast(sq, blockers, nw) | cast(sq, blockers, se) | cast(sq, blockers, sw);
}

constexpr Bitboard rook(const int sq, const Bitboard blockers) {
    return cast(sq, blockers, north) | cast(sq, blockers, east) | cast(sq, blockers, south) | cast(sq, blockers, west);
}

constexpr Bitboard king(const int sq, const Bitboard) {
    return adjacent(Bitboard(1ULL << sq));
}

}  // namespace chess::raycast

#endif

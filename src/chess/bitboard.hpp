#ifndef CHESS_BITBOARD_HPP
#define CHESS_BITBOARD_HPP

#include <cstdint>
#include <ostream>

namespace chess {

using Bitboard = std::uint64_t;

[[nodiscard]] constexpr Bitboard flip(Bitboard bb) noexcept {
    return __builtin_bswap64(bb);
}

[[nodiscard]] constexpr int lsbll(Bitboard bb) noexcept {
    return __builtin_ctzll(bb);
}

[[nodiscard]] constexpr auto count(Bitboard bb) noexcept {
    return __builtin_popcountll(bb);
}

[[nodiscard]] constexpr Bitboard north(Bitboard bb) noexcept {
    return bb << 8;
}

[[nodiscard]] constexpr Bitboard south(Bitboard bb) noexcept {
    return bb >> 8;
}

[[nodiscard]] constexpr Bitboard east(Bitboard bb) noexcept {
    return (bb << 1) & ~0x0101010101010101ULL;
}

[[nodiscard]] constexpr Bitboard west(Bitboard bb) noexcept {
    return (bb >> 1) & ~0x8080808080808080ULL;
}

[[nodiscard]] constexpr Bitboard nw(Bitboard bb) noexcept {
    return (bb << 7) & ~0x8080808080808080ULL;
}

[[nodiscard]] constexpr Bitboard ne(Bitboard bb) noexcept {
    return (bb << 9) & ~0x0101010101010101ULL;
}

[[nodiscard]] constexpr Bitboard sw(Bitboard bb) noexcept {
    return (bb >> 9) & ~0x8080808080808080ULL;
}

[[nodiscard]] constexpr Bitboard se(Bitboard bb) noexcept {
    return (bb >> 7) & ~0x0101010101010101ULL;
}

[[nodiscard]] constexpr Bitboard adjacent(Bitboard bb) noexcept {
    return (bb << 8) | (bb >> 8) | (((bb >> 1) | (bb >> 9) | (bb << 7)) & 0x7F7F7F7F7F7F7F7FULL) |
           (((bb << 1) | (bb << 9) | (bb >> 7)) & 0xFEFEFEFEFEFEFEFEULL);
}

[[nodiscard]] constexpr Bitboard knight(Bitboard bb) noexcept {
    return (((bb << 15) | (bb >> 17)) & 0x7F7F7F7F7F7F7F7FULL) | (((bb << 17) | (bb >> 15)) & 0xFEFEFEFEFEFEFEFEULL) |
           (((bb << 10) | (bb >> 6)) & 0xFCFCFCFCFCFCFCFCULL) | (((bb << 6) | (bb >> 10)) & 0x3F3F3F3F3F3F3F3FULL);
}

[[maybe_unused]] static std::ostream &operator<<(std::ostream &os, const Bitboard &bb) {
    for (int r = 7; r >= 0; --r) {
        for (int f = 0; f < 8; ++f) {
            os << (bb & Bitboard(1ULL << (8 * r + f)) ? 1 : 0);
        }
        os << '\n';
    }
    return os;
}

}  // namespace chess

#endif

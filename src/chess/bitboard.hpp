#ifndef CHESS_BITBOARD_HPP
#define CHESS_BITBOARD_HPP

#include <cstdint>
#include <ostream>

namespace chess {

class BitboardIterator {
   public:
    constexpr BitboardIterator(const std::uint64_t &data) : m_data{data} {
    }

    [[nodiscard]] constexpr int operator*() const noexcept {
        return __builtin_ctzll(m_data);
    }

    constexpr BitboardIterator operator++() noexcept {
        m_data &= m_data - 1;
        return *this;
    }

    [[nodiscard]] constexpr bool operator!=(const BitboardIterator &rhs) const noexcept {
        return m_data != rhs.m_data;
    }

   private:
    std::uint64_t m_data;
};

class Bitboard {
   public:
    constexpr Bitboard() : m_mask{} {
    }

    constexpr Bitboard(const int sq) : m_mask{1ULL << sq} {
    }

    constexpr Bitboard(const long long unsigned int mask) : m_mask{mask} {
    }

    constexpr Bitboard(const std::uint64_t mask) : m_mask{mask} {
    }

    void flip() noexcept {
        m_mask = __builtin_bswap64(m_mask);
    }

    void clear() noexcept {
        m_mask = 0;
    }

    constexpr Bitboard operator|=(const Bitboard &rhs) noexcept {
        m_mask |= rhs.m_mask;
        return *this;
    }

    constexpr Bitboard operator^=(const Bitboard &rhs) noexcept {
        m_mask ^= rhs.m_mask;
        return *this;
    }

    constexpr Bitboard operator&=(const Bitboard &rhs) noexcept {
        m_mask &= rhs.m_mask;
        return *this;
    }

    [[nodiscard]] constexpr Bitboard flipped() const noexcept {
        return __builtin_bswap64(m_mask);
    }

    [[nodiscard]] constexpr int lsbll() const noexcept {
        return __builtin_ctzll(m_mask);
    }

    [[nodiscard]] constexpr auto count() const noexcept {
        return __builtin_popcountll(m_mask);
    }

    [[nodiscard]] constexpr operator bool() const noexcept {
        return m_mask != 0;
    }

    [[nodiscard]] constexpr bool operator==(const Bitboard &rhs) const noexcept {
        return m_mask == rhs.m_mask;
    }

    [[nodiscard]] constexpr Bitboard north() const noexcept {
        return m_mask << 8;
    }

    [[nodiscard]] constexpr Bitboard south() const noexcept {
        return m_mask >> 8;
    }

    [[nodiscard]] constexpr Bitboard east() const noexcept {
        return (m_mask << 1) & ~0x0101010101010101ULL;
    }

    [[nodiscard]] constexpr Bitboard west() const noexcept {
        return (m_mask >> 1) & ~0x8080808080808080ULL;
    }

    [[nodiscard]] constexpr Bitboard nw() const noexcept {
        return (m_mask << 7) & ~0x8080808080808080ULL;
    }

    [[nodiscard]] constexpr Bitboard ne() const noexcept {
        return (m_mask << 9) & ~0x0101010101010101ULL;
    }

    [[nodiscard]] constexpr Bitboard sw() const noexcept {
        return (m_mask >> 9) & ~0x8080808080808080ULL;
    }

    [[nodiscard]] constexpr Bitboard se() const noexcept {
        return (m_mask >> 7) & ~0x0101010101010101ULL;
    }

    [[nodiscard]] constexpr Bitboard adjacent() const noexcept {
        return (m_mask << 8) | (m_mask >> 8) |
               (((m_mask >> 1) | (m_mask >> 9) | (m_mask << 7)) & 0x7F7F7F7F7F7F7F7FULL) |
               (((m_mask << 1) | (m_mask << 9) | (m_mask >> 7)) & 0xFEFEFEFEFEFEFEFEULL);
    }

    [[nodiscard]] constexpr Bitboard knight() const noexcept {
        return (((m_mask << 15) | (m_mask >> 17)) & 0x7F7F7F7F7F7F7F7FULL) |
               (((m_mask << 17) | (m_mask >> 15)) & 0xFEFEFEFEFEFEFEFEULL) |
               (((m_mask << 10) | (m_mask >> 6)) & 0xFCFCFCFCFCFCFCFCULL) |
               (((m_mask << 6) | (m_mask >> 10)) & 0x3F3F3F3F3F3F3F3FULL);
    }

    [[nodiscard]] constexpr Bitboard operator|(const Bitboard &rhs) const noexcept {
        return m_mask | rhs.m_mask;
    }

    [[nodiscard]] constexpr Bitboard operator^(const Bitboard &rhs) const noexcept {
        return m_mask ^ rhs.m_mask;
    }

    [[nodiscard]] constexpr Bitboard operator&(const Bitboard &rhs) const noexcept {
        return m_mask & rhs.m_mask;
    }

    [[nodiscard]] constexpr Bitboard operator~() const noexcept {
        return ~m_mask;
    }

    [[nodiscard]] constexpr BitboardIterator begin() const noexcept {
        return BitboardIterator{m_mask};
    }

    [[nodiscard]] constexpr BitboardIterator end() const noexcept {
        return BitboardIterator{0};
    }

   private:
    std::uint64_t m_mask;
};

static std::ostream &operator<<(std::ostream &os, const Bitboard &bb) {
    for (int r = 7; r >= 0; --r) {
        for (int f = 0; f < 8; ++f) {
            os << (bb & Bitboard(8 * r + f) ? 1 : 0);
        }
        os << '\n';
    }
    return os;
}

}  // namespace chess

#endif

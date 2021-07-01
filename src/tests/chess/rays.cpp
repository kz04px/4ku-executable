#include <catch2/catch.hpp>
#include <chess/raycast.hpp>

TEST_CASE("raycast north") {
    const std::pair<chess::Bitboard, chess::Bitboard> tests[] = {
        {0x0ULL, 0x1010101000000000ULL},
        {0x10000000000000ULL, 0x10101000000000ULL},
    };

    REQUIRE(chess::raycast::N(0, 0x0ULL) == chess::Bitboard(0x101010101010100ULL));

    for (const auto &[blockers, mask] : tests) {
        REQUIRE(chess::raycast::N(28, blockers) == mask);
    }
}

TEST_CASE("raycast west") {
    const std::pair<chess::Bitboard, chess::Bitboard> tests[] = {
        {0x0ULL, 0xF000000ULL},
    };

    for (const auto &[blockers, mask] : tests) {
        REQUIRE(chess::raycast::W(28, blockers) == mask);
    }
}

TEST_CASE("raycast bishop") {
    const std::pair<chess::Bitboard, chess::Bitboard> tests[] = {
        {0x0ULL, 0x182442800284482ULL},
        {0x440000004400ULL, 0x442800284400ULL},
    };

    for (const auto &[blockers, mask] : tests) {
        REQUIRE(chess::raycast::bishop(28, blockers) == mask);
    }
}

TEST_CASE("raycast rook") {
    const std::pair<chess::Bitboard, chess::Bitboard> tests[] = {
        {0x0ULL, 0x10101010EF101010ULL},
    };

    for (const auto &[blockers, mask] : tests) {
        REQUIRE(chess::raycast::rook(28, blockers) == mask);
    }
}

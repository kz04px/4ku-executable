#include <catch2/catch.hpp>
#include <chess/attack.hpp>
#include <chess/bitboard.hpp>
#include <chess/fen.hpp>
#include <chess/makemove.hpp>
#include <chess/move.hpp>
#include <chess/movegen.hpp>
#include <chess/position.hpp>
#include <chess/raycast.hpp>
#include <chess/square.hpp>
#include <string>

void valid(const chess::Move &move) {
    const auto supermove = chess::raycast::knight(move.from, 0x0ULL) | chess::raycast::bishop(move.from, 0x0ULL) |
                           chess::raycast::rook(move.from, 0x0ULL);
    REQUIRE(move.to != move.from);
    REQUIRE(chess::Bitboard(1ULL << move.to) & supermove);
    REQUIRE(move.promo != chess::Piece::Pawn);
    REQUIRE(move.promo != chess::Piece::King);
    REQUIRE((move.to >= chess::Square::a8 || move.promo == chess::Piece::None));
}

void test(chess::Position &pos, const int depth) {
    if (depth == 0) {
        return;
    }

    chess::Move moves[256];
    int num_moves = 0;
    chess::movegen(pos, moves, num_moves, true);
    chess::movegen(pos, moves, num_moves, false);

    for (int i = 0; i < num_moves; ++i) {
        auto npos = pos;

        valid(moves[i]);

        // Check move legality
        if (!chess::makemove(npos, moves[i])) {
            continue;
        }

        test(pos, depth - 1);
    }
}

TEST_CASE("Move validity") {
    const std::string fens[] = {
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0",
        "2rq1rk1/pp1bppbp/2np1np1/8/3NP3/1BN1BP2/PPPQ2PP/2KR3R b - - 8",
        "2rqr1k1/pp1bppbp/3p1np1/4n3/3NP2P/1BN1BP2/PPPQ2P1/1K1R3R b - - 0",
        "rnbqkbnr/ppp1pppp/8/8/3pP3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0",
        "4k3/2b3q1/3P1P2/4K3/3P1P2/2b3q1/8/8 w - - 0 1",
        "k7/8/4r3/3pP3/8/8/8/4K3 w - d6 0 1",
        "r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1",
        "r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1",
    };

    chess::Position pos;

    for (const auto &fen : fens) {
        INFO(fen);
        set_fen(pos, fen);
        test(pos, 3);
    }
}

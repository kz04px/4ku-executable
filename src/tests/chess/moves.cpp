#include <catch2/catch.hpp>
#include <chess/attack.hpp>
#include <chess/bitboard.hpp>
#include <chess/fen.hpp>
#include <chess/makemove.hpp>
#include <chess/move.hpp>
#include <chess/movegen.hpp>
#include <chess/position.hpp>
#include <chess/square.hpp>
#include <string>

void valid(const chess::Move &move) {
    REQUIRE(move.to != move.from);
    REQUIRE(move.piece != chess::Piece::None);
    REQUIRE(move.promo != chess::Piece::Pawn);
    REQUIRE(move.promo != chess::Piece::King);
    REQUIRE(move.captured != chess::Piece::King);

    switch (move.type) {
        case chess::Move::Type::Quiet:
            REQUIRE(move.promo == chess::Piece::None);
            REQUIRE(move.captured == chess::Piece::None);
            break;
        case chess::Move::Type::Double:
            REQUIRE((Square::a4 <= move.to && move.to <= Square::h4));
            REQUIRE((Square::a2 <= move.from && move.from <= Square::h2));
            REQUIRE(move.to - move.from == 16);
            REQUIRE(move.piece == chess::Piece::Pawn);
            REQUIRE(move.promo == chess::Piece::None);
            REQUIRE(move.captured == chess::Piece::None);
            break;
        case chess::Move::Type::Capture:
            REQUIRE(move.promo == chess::Piece::None);
            REQUIRE(move.captured != chess::Piece::None);
            break;
        case chess::Move::Type::Enpassant:
            REQUIRE((Square::a6 <= move.to && move.to <= Square::h6));
            REQUIRE((Square::a5 <= move.from && move.from <= Square::h5));
            REQUIRE(((move.to - move.from == 7) || (move.to - move.from == 9)));
            REQUIRE(move.piece == chess::Piece::Pawn);
            REQUIRE(move.promo == chess::Piece::None);
            REQUIRE(move.captured == chess::Piece::Pawn);
            break;
        case chess::Move::Type::Ksc:
            REQUIRE(move.from == Square::e1);
            REQUIRE(move.to == Square::g1);
            REQUIRE(move.piece == chess::Piece::King);
            REQUIRE(move.promo == chess::Piece::None);
            REQUIRE(move.captured == chess::Piece::None);
            break;
        case chess::Move::Type::Qsc:
            REQUIRE(move.from == Square::e1);
            REQUIRE(move.to == Square::c1);
            REQUIRE(move.piece == chess::Piece::King);
            REQUIRE(move.promo == chess::Piece::None);
            REQUIRE(move.captured == chess::Piece::None);
            break;
        case chess::Move::Type::Promo:
            REQUIRE((Square::a8 <= move.to && move.to <= Square::h8));
            REQUIRE((Square::a7 <= move.from && move.from <= Square::h7));
            REQUIRE(move.to - move.from == 8);
            REQUIRE(move.piece == chess::Piece::Pawn);
            REQUIRE(move.promo != chess::Piece::None);
            REQUIRE(move.captured == chess::Piece::None);
            break;
        case chess::Move::Type::Promocapture:
            REQUIRE((Square::a8 <= move.to && move.to <= Square::h8));
            REQUIRE((Square::a7 <= move.from && move.from <= Square::h7));
            REQUIRE(((move.to - move.from == 7) || (move.to - move.from == 9)));
            REQUIRE(move.piece == chess::Piece::Pawn);
            REQUIRE(move.promo != chess::Piece::None);
            REQUIRE(move.captured != chess::Piece::None);
            break;
        default:
            FAIL();
            break;
    }
}

void test(chess::Position &pos, const int depth) {
    if (depth == 0) {
        return;
    }

    chess::Move moves[256];
    const int num_moves = chess::movegen(pos, moves);

    for (int i = 0; i < num_moves; ++i) {
        auto npos = pos;

        valid(moves[i]);

        chess::makemove(npos, moves[i]);

        const int ksq = chess::lsbll(npos.colour[1] & npos.pieces[static_cast<int>(chess::Piece::King)]);

        if (chess::attacked(npos, ksq, false)) {
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

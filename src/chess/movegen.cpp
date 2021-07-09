#include "movegen.hpp"
#include "attack.hpp"
#include "move.hpp"
#include "piece.hpp"
#include "position.hpp"
#include "raycast.hpp"
#include "square.hpp"

namespace chess {

[[nodiscard]] int generate_piece_moves(Move *movelist,
                                       const Position &pos,
                                       const Piece piece,
                                       Bitboard (*func)(int, Bitboard)) {
    int num_moves = 0;

    auto copy = pos.colour[0] & pos.pieces[static_cast<int>(piece)];
    while (copy) {
        auto fr = lsbll(copy);
        copy &= copy - 1;

        auto moves = func(fr, pos.colour[0] | pos.colour[1]);
        moves &= ~pos.colour[0];

        while (moves) {
            auto to = lsbll(moves);
            moves &= moves - 1;

            const auto captured = piece_on(pos, to);
            if (captured != Piece::None) {
                movelist[num_moves] = Move{Move::Type::Capture, piece, fr, to, captured, Piece::None};
            } else {
                movelist[num_moves] = Move{Move::Type::Quiet, piece, fr, to, Piece::None, Piece::None};
            }
            num_moves++;
        }
    }

    return num_moves;
}

int movegen(const Position &pos, Move *movelist) {
    int num_moves = 0;

    const auto empty = ~(pos.colour[0] | pos.colour[1]);
    const auto pawns = pos.colour[0] & pos.pieces[static_cast<int>(Piece::Pawn)];

    // Pawns -- Single
    auto copy = north(pawns) & empty;
    while (copy) {
        auto to = lsbll(copy);
        copy &= copy - 1;

        // Promotion
        if (to >= Square::a8) {
            movelist[num_moves + 0] = Move{Move::Type::Promo, Piece::Pawn, to - 8, to, Piece::None, Piece::Queen};
            movelist[num_moves + 1] = Move{Move::Type::Promo, Piece::Pawn, to - 8, to, Piece::None, Piece::Rook};
            movelist[num_moves + 2] = Move{Move::Type::Promo, Piece::Pawn, to - 8, to, Piece::None, Piece::Bishop};
            movelist[num_moves + 3] = Move{Move::Type::Promo, Piece::Pawn, to - 8, to, Piece::None, Piece::Knight};
            num_moves += 4;
        } else {
            movelist[num_moves] = Move{Move::Type::Quiet, Piece::Pawn, to - 8, to, Piece::None, Piece::None};
            num_moves++;
        }
    }

    // Pawns -- Double
    copy = south((south(empty) & empty)) & pawns & Bitboard(0xFF00ULL);
    while (copy) {
        auto fr = lsbll(copy);
        copy &= copy - 1;

        movelist[num_moves] = Move{Move::Type::Double, Piece::Pawn, fr, fr + 16, Piece::None, Piece::None};
        num_moves++;
    }

    // Pawns -- Captures
    copy = ne(pawns) & pos.colour[1];
    while (copy) {
        auto to = lsbll(copy);
        copy &= copy - 1;

        // Promotion
        if (to >= Square::a8) {
            movelist[num_moves + 0] =
                Move{Move::Type::Promocapture, Piece::Pawn, to - 9, to, piece_on(pos, to), Piece::Queen};
            movelist[num_moves + 1] =
                Move{Move::Type::Promocapture, Piece::Pawn, to - 9, to, piece_on(pos, to), Piece::Rook};
            movelist[num_moves + 2] =
                Move{Move::Type::Promocapture, Piece::Pawn, to - 9, to, piece_on(pos, to), Piece::Bishop};
            movelist[num_moves + 3] =
                Move{Move::Type::Promocapture, Piece::Pawn, to - 9, to, piece_on(pos, to), Piece::Knight};
            num_moves += 4;
        } else {
            movelist[num_moves] = Move{Move::Type::Capture, Piece::Pawn, to - 9, to, piece_on(pos, to), Piece::None};
            num_moves++;
        }
    }

    // Pawns -- Captures
    copy = nw(pawns) & pos.colour[1];
    while (copy) {
        auto to = lsbll(copy);
        copy &= copy - 1;

        // Promotion
        if (to >= Square::a8) {
            movelist[num_moves + 0] =
                Move{Move::Type::Promocapture, Piece::Pawn, to - 7, to, piece_on(pos, to), Piece::Queen};
            movelist[num_moves + 1] =
                Move{Move::Type::Promocapture, Piece::Pawn, to - 7, to, piece_on(pos, to), Piece::Rook};
            movelist[num_moves + 2] =
                Move{Move::Type::Promocapture, Piece::Pawn, to - 7, to, piece_on(pos, to), Piece::Bishop};
            movelist[num_moves + 3] =
                Move{Move::Type::Promocapture, Piece::Pawn, to - 7, to, piece_on(pos, to), Piece::Knight};
            num_moves += 4;
        } else {
            movelist[num_moves] = Move{Move::Type::Capture, Piece::Pawn, to - 7, to, piece_on(pos, to), Piece::None};
            num_moves++;
        }
    }

    // En passant
    if (pos.ep != -1) {
        const auto bb = Bitboard(1ULL << (Square::a6 + pos.ep));
        if (bb & ne(pawns)) {
            movelist[num_moves] = Move{Move::Type::Enpassant,
                                       Piece::Pawn,
                                       Square::a6 + pos.ep - 9,
                                       Square::a6 + pos.ep,
                                       Piece::Pawn,
                                       Piece::None};
            num_moves++;
        }
        if (bb & nw(pawns)) {
            movelist[num_moves] = Move{Move::Type::Enpassant,
                                       Piece::Pawn,
                                       Square::a6 + pos.ep - 7,
                                       Square::a6 + pos.ep,
                                       Piece::Pawn,
                                       Piece::None};
            num_moves++;
        }
    }

    num_moves += generate_piece_moves(&movelist[num_moves], pos, Piece::Knight, raycast::knight);
    num_moves += generate_piece_moves(&movelist[num_moves], pos, Piece::Bishop, raycast::bishop);
    num_moves += generate_piece_moves(&movelist[num_moves], pos, Piece::Queen, raycast::bishop);
    num_moves += generate_piece_moves(&movelist[num_moves], pos, Piece::Rook, raycast::rook);
    num_moves += generate_piece_moves(&movelist[num_moves], pos, Piece::Queen, raycast::rook);
    num_moves += generate_piece_moves(&movelist[num_moves], pos, Piece::King, raycast::king);

    const auto all = pos.colour[0] | pos.colour[1];

    // Castling
    if (pos.castling[0] && !(all & Bitboard(0x60ULL)) && !attacked(pos, Square::e1, true) &&
        !attacked(pos, Square::f1, true) && !attacked(pos, Square::g1, true)) {
        movelist[num_moves] = Move{Move::Type::Ksc, Piece::King, Square::e1, Square::g1, Piece::None, Piece::None};
        num_moves++;
    }
    if (pos.castling[1] && !(all & Bitboard(0xEULL)) && !attacked(pos, Square::e1, true) &&
        !attacked(pos, Square::d1, true) && !attacked(pos, Square::c1, true)) {
        movelist[num_moves] = Move{Move::Type::Qsc, Piece::King, Square::e1, Square::c1, Piece::None, Piece::None};
        num_moves++;
    }

    return num_moves;
}

}  // namespace chess

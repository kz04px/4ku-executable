#include "movegen.hpp"
#include "attack.hpp"
#include "move.hpp"
#include "piece.hpp"
#include "position.hpp"
#include "raycast.hpp"

namespace chess {

int movegen(const Position &pos, Move *movelist) {
    int num_moves = 0;

    const auto empty = ~(pos.colour[0] | pos.colour[1]);
    const auto pawns = pos.colour[0] & pos.pieces[static_cast<int>(Piece::Pawn)];

    // Pawns -- Single
    for (const auto &to : pawns.north() & empty) {
        // Promotion
        if (to >= 56) {
            movelist[num_moves + 0] = Move(Move::Type::Promo, Piece::Pawn, to - 8, to);
            movelist[num_moves + 0].promo = Piece::Queen;
            movelist[num_moves + 1] = Move(Move::Type::Promo, Piece::Pawn, to - 8, to);
            movelist[num_moves + 1].promo = Piece::Rook;
            movelist[num_moves + 2] = Move(Move::Type::Promo, Piece::Pawn, to - 8, to);
            movelist[num_moves + 2].promo = Piece::Bishop;
            movelist[num_moves + 3] = Move(Move::Type::Promo, Piece::Pawn, to - 8, to);
            movelist[num_moves + 3].promo = Piece::Knight;
            num_moves += 4;
        } else {
            movelist[num_moves] = Move(Move::Type::Quiet, Piece::Pawn, to - 8, to);
            num_moves++;
        }
    }

    // Pawns -- Double
    for (const auto &fr : (empty.south() & empty).south() & pawns & Bitboard(0xFF00ULL)) {
        movelist[num_moves] = Move(Move::Type::Double, Piece::Pawn, fr, fr + 16);
        num_moves++;
    }

    // Pawns -- Captures
    for (const auto &to : pawns.north().east() & pos.colour[1]) {
        // Promotion
        if (to >= 56) {
            movelist[num_moves + 0] = Move(Move::Type::Promocapture, Piece::Pawn, to - 9, to, piece_on(pos, to));
            movelist[num_moves + 0].promo = Piece::Queen;
            movelist[num_moves + 1] = Move(Move::Type::Promocapture, Piece::Pawn, to - 9, to, piece_on(pos, to));
            movelist[num_moves + 1].promo = Piece::Rook;
            movelist[num_moves + 2] = Move(Move::Type::Promocapture, Piece::Pawn, to - 9, to, piece_on(pos, to));
            movelist[num_moves + 2].promo = Piece::Bishop;
            movelist[num_moves + 3] = Move(Move::Type::Promocapture, Piece::Pawn, to - 9, to, piece_on(pos, to));
            movelist[num_moves + 3].promo = Piece::Knight;
            num_moves += 4;
        } else {
            movelist[num_moves] = Move(Move::Type::Capture, Piece::Pawn, to - 9, to, piece_on(pos, to));
            num_moves++;
        }
    }
    for (const auto &to : pawns.north().west() & pos.colour[1]) {
        // Promotion
        if (to >= 56) {
            movelist[num_moves + 0] = Move(Move::Type::Promocapture, Piece::Pawn, to - 7, to, piece_on(pos, to));
            movelist[num_moves + 0].promo = Piece::Queen;
            movelist[num_moves + 1] = Move(Move::Type::Promocapture, Piece::Pawn, to - 7, to, piece_on(pos, to));
            movelist[num_moves + 1].promo = Piece::Rook;
            movelist[num_moves + 2] = Move(Move::Type::Promocapture, Piece::Pawn, to - 7, to, piece_on(pos, to));
            movelist[num_moves + 2].promo = Piece::Bishop;
            movelist[num_moves + 3] = Move(Move::Type::Promocapture, Piece::Pawn, to - 7, to, piece_on(pos, to));
            movelist[num_moves + 3].promo = Piece::Knight;
            num_moves += 4;
        } else {
            movelist[num_moves] = Move(Move::Type::Capture, Piece::Pawn, to - 7, to, piece_on(pos, to));
            num_moves++;
        }
    }

    // En passant
    if (pos.ep != -1) {
        const auto bb = Bitboard(40 + pos.ep);
        if (bb.south().west() & pawns) {
            movelist[num_moves] = Move(Move::Type::Enpassant, Piece::Pawn, 40 + pos.ep - 9, 40 + pos.ep);
            num_moves++;
        }
        if (bb.south().east() & pawns) {
            movelist[num_moves] = Move(Move::Type::Enpassant, Piece::Pawn, 40 + pos.ep - 7, 40 + pos.ep);
            num_moves++;
        }
    }

    // Knights
    for (const auto &fr : pos.colour[0] & pos.pieces[static_cast<int>(Piece::Knight)]) {
        const auto bb = Bitboard(fr);
        auto moves = bb.knight();
        moves &= ~pos.colour[0];

        for (const auto to : moves) {
            const auto captured = piece_on(pos, to);
            const auto capture = captured != Piece::None;
            if (capture) {
                movelist[num_moves] = Move(Move::Type::Capture, Piece::Knight, fr, to, captured);
            } else {
                movelist[num_moves] = Move(Move::Type::Quiet, Piece::Knight, fr, to);
            }
            num_moves++;
        }
    }

    // Bishops
    for (const auto &fr : pos.colour[0] & pos.pieces[static_cast<int>(Piece::Bishop)]) {
        auto moves = raycast::bishop(fr, pos.colour[0] | pos.colour[1]);
        moves &= ~pos.colour[0];

        for (const auto to : moves) {
            const auto captured = piece_on(pos, to);
            const auto capture = captured != Piece::None;
            if (capture) {
                movelist[num_moves] = Move(Move::Type::Capture, Piece::Bishop, fr, to, captured);
            } else {
                movelist[num_moves] = Move(Move::Type::Quiet, Piece::Bishop, fr, to);
            }
            num_moves++;
        }
    }

    // Rooks
    for (const auto &fr : pos.colour[0] & pos.pieces[static_cast<int>(Piece::Rook)]) {
        auto moves = raycast::rook(fr, pos.colour[0] | pos.colour[1]);
        moves &= ~pos.colour[0];

        for (const auto to : moves) {
            const auto captured = piece_on(pos, to);
            const auto capture = captured != Piece::None;
            if (capture) {
                movelist[num_moves] = Move(Move::Type::Capture, Piece::Rook, fr, to, captured);
            } else {
                movelist[num_moves] = Move(Move::Type::Quiet, Piece::Rook, fr, to);
            }
            num_moves++;
        }
    }

    // Queens
    for (const auto &fr : pos.colour[0] & pos.pieces[static_cast<int>(Piece::Queen)]) {
        auto moves =
            raycast::rook(fr, pos.colour[0] | pos.colour[1]) | raycast::bishop(fr, pos.colour[0] | pos.colour[1]);
        moves &= ~pos.colour[0];

        for (const auto to : moves) {
            const auto captured = piece_on(pos, to);
            const auto capture = captured != Piece::None;
            if (capture) {
                movelist[num_moves] = Move(Move::Type::Capture, Piece::Queen, fr, to, captured);
            } else {
                movelist[num_moves] = Move(Move::Type::Quiet, Piece::Queen, fr, to);
            }
            num_moves++;
        }
    }

    // King
    for (const auto &fr : pos.colour[0] & pos.pieces[static_cast<int>(Piece::King)]) {
        for (const auto &to : Bitboard(fr).adjacent() & ~pos.colour[0]) {
            const auto captured = piece_on(pos, to);
            const auto capture = captured != Piece::None;
            if (capture) {
                movelist[num_moves] = Move(Move::Type::Capture, Piece::King, fr, to, captured);
            } else {
                movelist[num_moves] = Move(Move::Type::Quiet, Piece::King, fr, to);
            }
            num_moves++;
        }
    }

    const auto all = pos.colour[0] | pos.colour[1];

    // Castling
    if (pos.castling[0] && !(all & Bitboard(0x60ULL)) && !attacked(pos, 4, true) && !attacked(pos, 5, true) &&
        !attacked(pos, 6, true)) {
        movelist[num_moves] = Move(Move::Type::Ksc, Piece::King, 4, 6);
        num_moves++;
    }
    if (pos.castling[1] && !(all & Bitboard(0xEULL)) && !attacked(pos, 4, true) && !attacked(pos, 3, true) &&
        !attacked(pos, 2, true)) {
        movelist[num_moves] = Move(Move::Type::Qsc, Piece::King, 4, 2);
        num_moves++;
    }

    return num_moves;
}

}  // namespace chess

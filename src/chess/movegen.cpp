#include "movegen.hpp"
#include "attack.hpp"
#include "move.hpp"
#include "piece.hpp"
#include "position.hpp"
#include "raycast.hpp"
#include "square.hpp"

namespace chess {

void add_move(chess::Move *movelist,
              int &num_moves,
              const Move::Type type,
              const chess::Piece piece,
              const int from,
              const int to,
              const chess::Piece captured,
              const chess::Piece promo) {
    movelist[num_moves] = Move{type, piece, from, to, captured, promo};
    num_moves++;
}

void generate_piece_moves(Move *movelist,
                          int &num_moves,
                          const Position &pos,
                          const Piece piece,
                          Bitboard (*func)(int, Bitboard)) {
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
                add_move(movelist, num_moves, Move::Type::Capture, piece, fr, to, captured, Piece::None);
            } else {
                add_move(movelist, num_moves, Move::Type::Quiet, piece, fr, to, Piece::None, Piece::None);
            }
        }
    }
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
            add_move(movelist, num_moves, Move::Type::Promo, Piece::Pawn, to - 8, to, Piece::None, Piece::Queen);
            add_move(movelist, num_moves, Move::Type::Promo, Piece::Pawn, to - 8, to, Piece::None, Piece::Rook);
            add_move(movelist, num_moves, Move::Type::Promo, Piece::Pawn, to - 8, to, Piece::None, Piece::Bishop);
            add_move(movelist, num_moves, Move::Type::Promo, Piece::Pawn, to - 8, to, Piece::None, Piece::Knight);
        } else {
            add_move(movelist, num_moves, Move::Type::Quiet, Piece::Pawn, to - 8, to, Piece::None, Piece::None);
        }
    }

    // Pawns -- Double
    copy = south((south(empty) & empty)) & pawns & Bitboard(0xFF00ULL);
    while (copy) {
        auto fr = lsbll(copy);
        copy &= copy - 1;

        add_move(movelist, num_moves, Move::Type::Double, Piece::Pawn, fr, fr + 16, Piece::None, Piece::None);
    }

    // Pawns -- Captures
    copy = ne(pawns) & pos.colour[1];
    while (copy) {
        auto to = lsbll(copy);
        copy &= copy - 1;

        // Promotion
        if (to >= Square::a8) {
            add_move(movelist,
                     num_moves,
                     Move::Type::Promocapture,
                     Piece::Pawn,
                     to - 9,
                     to,
                     piece_on(pos, to),
                     Piece::Queen);
            add_move(
                movelist, num_moves, Move::Type::Promocapture, Piece::Pawn, to - 9, to, piece_on(pos, to), Piece::Rook);
            add_move(movelist,
                     num_moves,
                     Move::Type::Promocapture,
                     Piece::Pawn,
                     to - 9,
                     to,
                     piece_on(pos, to),
                     Piece::Bishop);
            add_move(movelist,
                     num_moves,
                     Move::Type::Promocapture,
                     Piece::Pawn,
                     to - 9,
                     to,
                     piece_on(pos, to),
                     Piece::Knight);
        } else {
            add_move(movelist, num_moves, Move::Type::Capture, Piece::Pawn, to - 9, to, piece_on(pos, to), Piece::None);
        }
    }

    // Pawns -- Captures
    copy = nw(pawns) & pos.colour[1];
    while (copy) {
        auto to = lsbll(copy);
        copy &= copy - 1;

        // Promotion
        if (to >= Square::a8) {
            add_move(movelist,
                     num_moves,
                     Move::Type::Promocapture,
                     Piece::Pawn,
                     to - 7,
                     to,
                     piece_on(pos, to),
                     Piece::Queen);
            add_move(
                movelist, num_moves, Move::Type::Promocapture, Piece::Pawn, to - 7, to, piece_on(pos, to), Piece::Rook);
            add_move(movelist,
                     num_moves,
                     Move::Type::Promocapture,
                     Piece::Pawn,
                     to - 7,
                     to,
                     piece_on(pos, to),
                     Piece::Bishop);
            add_move(movelist,
                     num_moves,
                     Move::Type::Promocapture,
                     Piece::Pawn,
                     to - 7,
                     to,
                     piece_on(pos, to),
                     Piece::Knight);
        } else {
            add_move(movelist, num_moves, Move::Type::Capture, Piece::Pawn, to - 7, to, piece_on(pos, to), Piece::None);
        }
    }

    // En passant
    if (pos.ep != -1) {
        const auto bb = Bitboard(1ULL << (Square::a6 + pos.ep));
        if (bb & ne(pawns)) {
            add_move(movelist,
                     num_moves,
                     Move::Type::Enpassant,
                     Piece::Pawn,
                     Square::a6 + pos.ep - 9,
                     Square::a6 + pos.ep,
                     Piece::Pawn,
                     Piece::None);
        }
        if (bb & nw(pawns)) {
            add_move(movelist,
                     num_moves,
                     Move::Type::Enpassant,
                     Piece::Pawn,
                     Square::a6 + pos.ep - 7,
                     Square::a6 + pos.ep,
                     Piece::Pawn,
                     Piece::None);
        }
    }

    generate_piece_moves(movelist, num_moves, pos, Piece::Knight, raycast::knight);
    generate_piece_moves(movelist, num_moves, pos, Piece::Bishop, raycast::bishop);
    generate_piece_moves(movelist, num_moves, pos, Piece::Queen, raycast::bishop);
    generate_piece_moves(movelist, num_moves, pos, Piece::Rook, raycast::rook);
    generate_piece_moves(movelist, num_moves, pos, Piece::Queen, raycast::rook);
    generate_piece_moves(movelist, num_moves, pos, Piece::King, raycast::king);

    const auto all = pos.colour[0] | pos.colour[1];

    // Castling
    if (pos.castling[0] && !(all & Bitboard(0x60ULL)) && !attacked(pos, Square::e1, true) &&
        !attacked(pos, Square::f1, true) && !attacked(pos, Square::g1, true)) {
        add_move(movelist, num_moves, Move::Type::Ksc, Piece::King, Square::e1, Square::g1, Piece::None, Piece::None);
    }
    if (pos.castling[1] && !(all & Bitboard(0xEULL)) && !attacked(pos, Square::e1, true) &&
        !attacked(pos, Square::d1, true) && !attacked(pos, Square::c1, true)) {
        add_move(movelist, num_moves, Move::Type::Qsc, Piece::King, Square::e1, Square::c1, Piece::None, Piece::None);
    }

    return num_moves;
}

}  // namespace chess

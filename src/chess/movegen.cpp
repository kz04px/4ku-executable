#include "movegen.hpp"
#include "attack.hpp"
#include "move.hpp"
#include "piece.hpp"
#include "position.hpp"
#include "raycast.hpp"
#include "square.hpp"

namespace chess {

void add_move(chess::Move *movelist, int &num_moves, const int from, const int to, const chess::Piece promo) {
    movelist[num_moves] = Move{from, to, promo};
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

            add_move(movelist, num_moves, fr, to, Piece::None);
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
            add_move(movelist, num_moves, to - 8, to, Piece::Queen);
            add_move(movelist, num_moves, to - 8, to, Piece::Rook);
            add_move(movelist, num_moves, to - 8, to, Piece::Bishop);
            add_move(movelist, num_moves, to - 8, to, Piece::Knight);
        } else {
            add_move(movelist, num_moves, to - 8, to, Piece::None);
        }
    }

    // Pawns -- Double
    copy = south((south(empty) & empty)) & pawns & Bitboard(0xFF00ULL);
    while (copy) {
        auto fr = lsbll(copy);
        copy &= copy - 1;

        add_move(movelist, num_moves, fr, fr + 16, Piece::None);
    }

    // Pawns -- Captures
    copy = ne(pawns) & pos.colour[1];
    while (copy) {
        auto to = lsbll(copy);
        copy &= copy - 1;

        // Promotion
        if (to >= Square::a8) {
            add_move(movelist, num_moves, to - 9, to, Piece::Queen);
            add_move(movelist, num_moves, to - 9, to, Piece::Rook);
            add_move(movelist, num_moves, to - 9, to, Piece::Bishop);
            add_move(movelist, num_moves, to - 9, to, Piece::Knight);
        } else {
            add_move(movelist, num_moves, to - 9, to, Piece::None);
        }
    }

    // Pawns -- Captures
    copy = nw(pawns) & pos.colour[1];
    while (copy) {
        auto to = lsbll(copy);
        copy &= copy - 1;

        // Promotion
        if (to >= Square::a8) {
            add_move(movelist, num_moves, to - 7, to, Piece::Queen);
            add_move(movelist, num_moves, to - 7, to, Piece::Rook);
            add_move(movelist, num_moves, to - 7, to, Piece::Bishop);
            add_move(movelist, num_moves, to - 7, to, Piece::Knight);
        } else {
            add_move(movelist, num_moves, to - 7, to, Piece::None);
        }
    }

    // En passant
    if (pos.ep != -1) {
        const auto bb = Bitboard(1ULL << (Square::a6 + pos.ep));
        if (bb & ne(pawns)) {
            add_move(movelist, num_moves, Square::a6 + pos.ep - 9, Square::a6 + pos.ep, Piece::None);
        }
        if (bb & nw(pawns)) {
            add_move(movelist, num_moves, Square::a6 + pos.ep - 7, Square::a6 + pos.ep, Piece::None);
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
        add_move(movelist, num_moves, Square::e1, Square::g1, Piece::None);
    }
    if (pos.castling[1] && !(all & Bitboard(0xEULL)) && !attacked(pos, Square::e1, true) &&
        !attacked(pos, Square::d1, true) && !attacked(pos, Square::c1, true)) {
        add_move(movelist, num_moves, Square::e1, Square::c1, Piece::None);
    }

    return num_moves;
}

}  // namespace chess

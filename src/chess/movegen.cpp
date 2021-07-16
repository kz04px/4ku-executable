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

void generate_pawn_moves(Move *movelist, int &num_moves, Bitboard to_mask, const int offset) {
    while (to_mask) {
        auto to = lsbll(to_mask);
        to_mask &= to_mask - 1;

        // Promotion
        if (to >= Square::a8) {
            add_move(movelist, num_moves, to + offset, to, Piece::Queen);
            add_move(movelist, num_moves, to + offset, to, Piece::Rook);
            add_move(movelist, num_moves, to + offset, to, Piece::Bishop);
            add_move(movelist, num_moves, to + offset, to, Piece::Knight);
        } else {
            add_move(movelist, num_moves, to + offset, to, Piece::None);
        }
    }
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

    const auto all = pos.colour[0] | pos.colour[1];
    const auto empty = ~all;
    const auto pawns = pos.colour[0] & pos.pieces[static_cast<int>(Piece::Pawn)];

    // Pawns
    generate_pawn_moves(movelist, num_moves, north(pawns) & empty, -8);
    generate_pawn_moves(movelist, num_moves, north(north(pawns & Bitboard(0xFF00ULL)) & empty) & empty, -16);
    generate_pawn_moves(movelist, num_moves, nw(pawns) & (pos.colour[1] | pos.ep), -7);
    generate_pawn_moves(movelist, num_moves, ne(pawns) & (pos.colour[1] | pos.ep), -9);
    // Others
    generate_piece_moves(movelist, num_moves, pos, Piece::Knight, raycast::knight);
    generate_piece_moves(movelist, num_moves, pos, Piece::Bishop, raycast::bishop);
    generate_piece_moves(movelist, num_moves, pos, Piece::Queen, raycast::bishop);
    generate_piece_moves(movelist, num_moves, pos, Piece::Rook, raycast::rook);
    generate_piece_moves(movelist, num_moves, pos, Piece::Queen, raycast::rook);
    generate_piece_moves(movelist, num_moves, pos, Piece::King, raycast::king);

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

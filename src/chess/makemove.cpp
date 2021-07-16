#include "makemove.hpp"
#include "move.hpp"
#include "piece.hpp"
#include "position.hpp"
#include "square.hpp"

namespace chess {

void makemove(Position &pos, const Move &move) {
    const auto piece = piece_on(pos, move.from);
    const auto captured = piece_on(pos, move.to);

    // Move our piece
    pos.colour[0] ^= Bitboard(1ULL << move.from) | Bitboard(1ULL << move.to);
    pos.pieces[static_cast<int>(piece)] ^= Bitboard(1ULL << move.from) | Bitboard(1ULL << move.to);

    // En passant
    if (piece == Piece::Pawn && Bitboard(1ULL << move.to) == pos.ep) {
        // Remove their pawn
        pos.colour[1] ^= Bitboard(1ULL << (move.to - 8));
        pos.pieces[static_cast<int>(Piece::Pawn)] ^= Bitboard(1ULL << (move.to - 8));
    }

    pos.ep = 0x0ULL;

    // Double
    if (piece == Piece::Pawn && move.to - move.from == 16) {
        pos.ep = Bitboard(1ULL << (move.from + 8));
    }

    // Remove their piece
    if (captured != Piece::None) {
        pos.colour[1] ^= Bitboard(1ULL << move.to);
        pos.pieces[static_cast<int>(captured)] ^= Bitboard(1ULL << move.to);
    }

    // King side castle
    if (piece == Piece::King && move.to - move.from == 2) {
        // Move the rook
        pos.colour[0] ^= Bitboard((1ULL << Square::h1) | (1ULL << Square::f1));
        pos.pieces[static_cast<int>(Piece::Rook)] ^= Bitboard((1ULL << Square::h1) | (1ULL << Square::f1));
    }

    // Queen side castle
    if (piece == Piece::King && move.to - move.from == -2) {
        // Move the rook
        pos.colour[0] ^= Bitboard((1ULL << Square::a1) | (1ULL << Square::d1));
        pos.pieces[static_cast<int>(Piece::Rook)] ^= Bitboard((1ULL << Square::a1) | (1ULL << Square::d1));
    }

    // Promo
    if (piece == Piece::Pawn && move.to >= Square::a8) {
        // Replace pawn with new piece
        pos.pieces[static_cast<int>(Piece::Pawn)] ^= Bitboard(1ULL << move.to);
        pos.pieces[static_cast<int>(move.promo)] ^= Bitboard(1ULL << move.to);
    }

    // Remove castling permissions
    pos.castling[0] &= (move.to != Square::h1 && move.from != Square::h1 && move.from != Square::e1);
    pos.castling[1] &= (move.to != Square::a1 && move.from != Square::a1 && move.from != Square::e1);
    pos.castling[2] &= (move.to != Square::h8 && move.from != Square::h8 && move.from != Square::e8);
    pos.castling[3] &= (move.to != Square::a8 && move.from != Square::a8 && move.from != Square::e8);

    // Flip the board
    flip(pos);
}

}  // namespace chess

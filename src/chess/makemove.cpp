#include "makemove.hpp"
#include "attack.hpp"
#include "move.hpp"
#include "piece.hpp"
#include "position.hpp"
#include "square.hpp"

namespace chess {

bool makemove(Position &pos, const Move &move) {
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

    // Castling
    if (piece == Piece::King) {
        const auto bb = move.to - move.from == 2 ? 0xa0ULL : move.to - move.from == -2 ? 0x9ULL : 0x0ULL;
        pos.colour[0] ^= bb;
        pos.pieces[static_cast<int>(Piece::Rook)] ^= bb;
    }

    // Promo
    if (piece == Piece::Pawn && move.to >= Square::a8) {
        // Replace pawn with new piece
        pos.pieces[static_cast<int>(Piece::Pawn)] ^= Bitboard(1ULL << move.to);
        pos.pieces[static_cast<int>(move.promo)] ^= Bitboard(1ULL << move.to);
    }

    // Remove castling permissions
    const auto changed = (1ULL << move.to) | (1ULL << move.from);
    pos.castling[0] &= !(changed & 0x90ULL);
    pos.castling[1] &= !(changed & 0x11ULL);
    pos.castling[2] &= !(changed & 0x9000000000000000ULL);
    pos.castling[3] &= !(changed & 0x1100000000000000ULL);

    // Flip the board
    flip(pos);

    // Did this move hang our king?
    const int ksq = lsbll(pos.colour[1] & pos.pieces[static_cast<int>(chess::Piece::King)]);
    return !attacked(pos, ksq, false);
}

}  // namespace chess

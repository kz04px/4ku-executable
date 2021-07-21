#include "makemove.hpp"
#include "attack.hpp"
#include "move.hpp"
#include "piece.hpp"
#include "position.hpp"
#include "square.hpp"

namespace chess {

bool makemove(Position &pos, const Move &move) {
    // Clear piece squares
    pos.colour[1] &= ~(Bitboard(1ULL << move.from) | Bitboard(1ULL << move.to));
    for (int i = 0; i < 6; ++i) {
        pos.pieces[i] &= ~(Bitboard(1ULL << move.from) | Bitboard(1ULL << move.to));
    }

    // Add our piece
    pos.colour[0] ^= Bitboard(1ULL << move.from) | Bitboard(1ULL << move.to);
    pos.pieces[static_cast<int>(move.place_piece)] ^= Bitboard(1ULL << move.to);

    // En passant
    if (move.place_piece == Piece::Pawn && Bitboard(1ULL << move.to) == pos.ep) {
        // Remove their pawn
        pos.colour[1] ^= pos.ep >> 8;
        pos.pieces[static_cast<int>(Piece::Pawn)] ^= pos.ep >> 8;
    }

    pos.ep = 0x0ULL;

    // Double
    if (move.place_piece == Piece::Pawn && move.to - move.from == 16) {
        pos.ep = Bitboard(1ULL << (move.from + 8));
    }

    // Castling
    if (move.place_piece == Piece::King) {
        const auto bb = move.to - move.from == 2 ? 0xa0ULL : move.to - move.from == -2 ? 0x9ULL : 0x0ULL;
        pos.colour[0] ^= bb;
        pos.pieces[static_cast<int>(Piece::Rook)] ^= bb;
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

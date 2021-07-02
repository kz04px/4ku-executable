#include "makemove.hpp"
#include "move.hpp"
#include "piece.hpp"
#include "position.hpp"

namespace chess {

void makemove(Position &pos, const Move &move) {
    // Move our piece
    pos.colour[0] ^= Bitboard(move.from) | Bitboard(move.to);
    pos.pieces[static_cast<int>(move.piece)] ^= Bitboard(move.from) | Bitboard(move.to);

    // Update history
    pos.history[pos.history_size].ep = pos.ep;
    pos.history[pos.history_size].castling[0] = pos.castling[0];
    pos.history[pos.history_size].castling[1] = pos.castling[1];
    pos.history[pos.history_size].castling[2] = pos.castling[2];
    pos.history[pos.history_size].castling[3] = pos.castling[3];
    pos.history_size++;

    pos.ep = -1;

    switch (move.type) {
        case Move::Type::Quiet:
            break;
        case Move::Type::Double:
            pos.ep = move.to % 8;
            break;
        case Move::Type::Capture:
            // Remove their piece
            pos.colour[1] ^= Bitboard(move.to);
            pos.pieces[static_cast<int>(move.captured)] ^= Bitboard(move.to);
            break;
        case Move::Type::Enpassant:
            // Remove their pawn
            pos.colour[1] ^= Bitboard(move.to - 8);
            pos.pieces[static_cast<int>(Piece::Pawn)] ^= Bitboard(move.to - 8);
            break;
        case Move::Type::Ksc:
            // Move the rook
            pos.colour[0] ^= Bitboard((1ULL << 7) | (1ULL << 5));
            pos.pieces[static_cast<int>(Piece::Rook)] ^= Bitboard((1ULL << 7) | (1ULL << 5));
            break;
        case Move::Type::Qsc:
            // Move the rook
            pos.colour[0] ^= Bitboard((1ULL << 0) | (1ULL << 3));
            pos.pieces[static_cast<int>(Piece::Rook)] ^= Bitboard((1ULL << 0) | (1ULL << 3));
            break;
        case Move::Type::Promo:
            // Replace pawn with new piece
            pos.pieces[static_cast<int>(Piece::Pawn)] ^= Bitboard(move.to);
            pos.pieces[static_cast<int>(move.promo)] ^= Bitboard(move.to);
            break;
        case Move::Type::Promocapture:
            // Replace pawn with new piece
            pos.pieces[static_cast<int>(Piece::Pawn)] ^= Bitboard(move.to);
            pos.pieces[static_cast<int>(move.promo)] ^= Bitboard(move.to);
            // Remove their piece
            pos.colour[1] ^= Bitboard(move.to);
            pos.pieces[static_cast<int>(move.captured)] ^= Bitboard(move.to);
            break;
        default:
            break;
    }

    // Remove castling permissions
    pos.castling[0] &= (move.to != 7 && move.from != 7 && move.from != 4);
    pos.castling[1] &= (move.to != 0 && move.from != 0 && move.from != 4);
    pos.castling[2] &= (move.to != 63 && move.from != 63 && move.from != 60);
    pos.castling[3] &= (move.to != 56 && move.from != 56 && move.from != 60);

    // Flip the board
    flip(pos);
}

}  // namespace chess

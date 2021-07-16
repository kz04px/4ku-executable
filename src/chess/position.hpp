#ifndef CHESS_POSITION_HPP
#define CHESS_POSITION_HPP

#include <ostream>
#include "bitboard.hpp"
#include "piece.hpp"

namespace chess {

struct Position {
    Bitboard colour[2] = {0xFFFFULL, 0xFFFF000000000000ULL};
    Bitboard pieces[6] = {0xFF00000000FF00ULL,
                          0x4200000000000042ULL,
                          0x2400000000000024ULL,
                          0x8100000000000081ULL,
                          0x800000000000008ULL,
                          0x1000000000000010ULL};
    Bitboard ep = 0x0ULL;
    int halfmoves = 0;
    bool castling[4] = {true, true, true, true};
    bool flipped = false;
};

static Piece piece_on(const Position &pos, const int sq) noexcept {
    const auto bb = Bitboard(1ULL << sq);
    for (int i = 0; i < 6; ++i) {
        if (pos.pieces[i] & bb) {
            return static_cast<Piece>(i);
        }
    }
    return Piece::None;
}

static int colour_on(const Position &pos, const int sq) {
    const auto bb = Bitboard(1ULL << sq);
    if (pos.colour[0] & bb) {
        return 0;
    } else if (pos.pieces[1] & bb) {
        return 1;
    } else {
        return 2;
    }
}

[[maybe_unused]] static std::ostream &operator<<(std::ostream &os, const Position &pos) {
    for (int r = 7; r >= 0; --r) {
        for (int f = 0; f < 8; ++f) {
            const int sq = 8 * r + f;
            const auto piece = piece_on(pos, sq);
            const auto white = colour_on(pos, sq) == 0;
            switch (piece) {
                case Piece::Pawn:
                    os << (white ? 'P' : 'p');
                    break;
                case Piece::Knight:
                    os << (white ? 'N' : 'n');
                    break;
                case Piece::Bishop:
                    os << (white ? 'B' : 'b');
                    break;
                case Piece::Rook:
                    os << (white ? 'R' : 'r');
                    break;
                case Piece::Queen:
                    os << (white ? 'Q' : 'q');
                    break;
                case Piece::King:
                    os << (white ? 'K' : 'k');
                    break;
                default:
                    os << '-';
                    break;
            }
        }
        os << '\n';
    }
    os << "Flipped: " << (pos.flipped ? "true" : "false") << "\n";
    return os;
}

[[maybe_unused]] static void flip(Position &pos) noexcept {
    pos.colour[0] = flip(pos.colour[0]);
    pos.colour[1] = flip(pos.colour[1]);

    for (int i = 0; i < 6; ++i) {
        pos.pieces[i] = flip(pos.pieces[i]);
    }

    pos.ep = flip(pos.ep);

    {
        const auto c = pos.colour[0];
        pos.colour[0] = pos.colour[1];
        pos.colour[1] = c;
    }
    {
        const auto c = pos.castling[0];
        pos.castling[0] = pos.castling[2];
        pos.castling[2] = c;
    }
    {
        const auto c = pos.castling[1];
        pos.castling[1] = pos.castling[3];
        pos.castling[3] = c;
    }
    pos.flipped = !pos.flipped;
}

}  // namespace chess

#endif

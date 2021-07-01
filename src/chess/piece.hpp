#ifndef CHESS_PIECE_HPP
#define CHESS_PIECE_HPP

#include <cstdint>

namespace chess {

enum class Piece : std::uint8_t
{
    Pawn = 0,
    Knight,
    Bishop,
    Rook,
    Queen,
    King,
    None,
};

}  // namespace chess

#endif

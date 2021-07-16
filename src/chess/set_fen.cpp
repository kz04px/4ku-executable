#include <sstream>
#include <string>
#include "fen.hpp"
#include "position.hpp"

namespace chess {

void set_fen(Position &pos, const std::string &fen) {
    if (fen == "startpos") {
        set_fen(pos, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        return;
    }

    // Clear
    pos.colour[0] = 0ULL;
    pos.colour[1] = 0ULL;
    pos.pieces[0] = 0ULL;
    pos.pieces[1] = 0ULL;
    pos.pieces[2] = 0ULL;
    pos.pieces[3] = 0ULL;
    pos.pieces[4] = 0ULL;
    pos.pieces[5] = 0ULL;
    pos.castling[0] = false;
    pos.castling[1] = false;
    pos.castling[2] = false;
    pos.castling[3] = false;
    pos.halfmoves = 0;

    std::stringstream ss{fen};
    std::string word;
    bool black_move = false;

    ss >> word;
    int i = 56;
    for (const auto &c : word) {
        switch (c) {
            case 'P':
                pos.colour[0] ^= Bitboard(1ULL << i);
                pos.pieces[static_cast<int>(Piece::Pawn)] ^= Bitboard(1ULL << i);
                i++;
                break;
            case 'p':
                pos.colour[1] ^= Bitboard(1ULL << i);
                pos.pieces[static_cast<int>(Piece::Pawn)] ^= Bitboard(1ULL << i);
                i++;
                break;
            case 'N':
                pos.colour[0] ^= Bitboard(1ULL << i);
                pos.pieces[static_cast<int>(Piece::Knight)] ^= Bitboard(1ULL << i);
                i++;
                break;
            case 'n':
                pos.colour[1] ^= Bitboard(1ULL << i);
                pos.pieces[static_cast<int>(Piece::Knight)] ^= Bitboard(1ULL << i);
                i++;
                break;
            case 'B':
                pos.colour[0] ^= Bitboard(1ULL << i);
                pos.pieces[static_cast<int>(Piece::Bishop)] ^= Bitboard(1ULL << i);
                i++;
                break;
            case 'b':
                pos.colour[1] ^= Bitboard(1ULL << i);
                pos.pieces[static_cast<int>(Piece::Bishop)] ^= Bitboard(1ULL << i);
                i++;
                break;
            case 'R':
                pos.colour[0] ^= Bitboard(1ULL << i);
                pos.pieces[static_cast<int>(Piece::Rook)] ^= Bitboard(1ULL << i);
                i++;
                break;
            case 'r':
                pos.colour[1] ^= Bitboard(1ULL << i);
                pos.pieces[static_cast<int>(Piece::Rook)] ^= Bitboard(1ULL << i);
                i++;
                break;
            case 'Q':
                pos.colour[0] ^= Bitboard(1ULL << i);
                pos.pieces[static_cast<int>(Piece::Queen)] ^= Bitboard(1ULL << i);
                i++;
                break;
            case 'q':
                pos.colour[1] ^= Bitboard(1ULL << i);
                pos.pieces[static_cast<int>(Piece::Queen)] ^= Bitboard(1ULL << i);
                i++;
                break;
            case 'K':
                pos.colour[0] ^= Bitboard(1ULL << i);
                pos.pieces[static_cast<int>(Piece::King)] ^= Bitboard(1ULL << i);
                i++;
                break;
            case 'k':
                pos.colour[1] ^= Bitboard(1ULL << i);
                pos.pieces[static_cast<int>(Piece::King)] ^= Bitboard(1ULL << i);
                i++;
                break;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
                i += c - '1' + 1;
                break;
            case '/':
                i -= 16;
                break;
            default:
                break;
        }
    }

    // Side to move
    ss >> word;
    if (word == "b") {
        black_move = true;
    }

    // Castling perms
    ss >> word;
    for (const auto &c : word) {
        if (c == 'K') {
            pos.castling[0] = true;
        } else if (c == 'Q') {
            pos.castling[1] = true;
        } else if (c == 'k') {
            pos.castling[2] = true;
        } else if (c == 'q') {
            pos.castling[3] = true;
        }
    }

    // En passant
    ss >> word;
    if (word != "-") {
        const int sq = word[0] - 'a' + 8 * (word[1] - '1');
        pos.ep = 1ULL << sq;
    }

    // Halfmove clock
    ss >> pos.halfmoves;

    // Fullmove clock
    // ss >> fullmove_clock_;

    // Flip the board if necessary
    if (black_move) {
        flip(pos);
    }
}

std::string get_fen(const Position &pos) {
    std::string fen;

    auto npos = pos;

    if (pos.flipped) {
        flip(npos);
    }

    // Part 1 -- Piece locations
    int empty = 0;
    for (int i = 56; i >= 0; ++i) {
        const auto bb = Bitboard(1ULL << i);

        if (npos.pieces[static_cast<int>(Piece::Pawn)] & bb) {
            if (empty > 0) {
                fen += std::to_string(empty);
            }
            empty = 0;
            if (npos.colour[0] & bb) {
                fen += "P";
            } else {
                fen += "p";
            }
        } else if (npos.pieces[static_cast<int>(Piece::Knight)] & bb) {
            if (empty > 0) {
                fen += std::to_string(empty);
            }
            empty = 0;
            if (npos.colour[0] & bb) {
                fen += "N";
            } else {
                fen += "n";
            }
        } else if (npos.pieces[static_cast<int>(Piece::Bishop)] & bb) {
            if (empty > 0) {
                fen += std::to_string(empty);
            }
            empty = 0;
            if (npos.colour[0] & bb) {
                fen += "B";
            } else {
                fen += "b";
            }
        } else if (npos.pieces[static_cast<int>(Piece::Rook)] & bb) {
            if (empty > 0) {
                fen += std::to_string(empty);
            }
            empty = 0;
            if (npos.colour[0] & bb) {
                fen += "R";
            } else {
                fen += "r";
            }
        } else if (npos.pieces[static_cast<int>(Piece::Queen)] & bb) {
            if (empty > 0) {
                fen += std::to_string(empty);
            }
            empty = 0;
            if (npos.colour[0] & bb) {
                fen += "Q";
            } else {
                fen += "q";
            }
        } else if (npos.pieces[static_cast<int>(Piece::King)] & bb) {
            if (empty > 0) {
                fen += std::to_string(empty);
            }
            empty = 0;
            if (npos.colour[0] & bb) {
                fen += "K";
            } else {
                fen += "k";
            }
        } else {
            empty++;
        }

        if (i % 8 == 7) {
            if (empty > 0) {
                fen += std::to_string(empty);
            }
            empty = 0;
            if (i > 7) {
                fen += "/";
            }
            i -= 16;
        }
    }

    // Part 2 -- Side to move
    if (pos.flipped) {
        fen += " b";
    } else {
        fen += " w";
    }

    // Part 3 -- Castling permissions
    {
        std::string part;
        if (npos.castling[0]) {
            part += "K";
        }
        if (npos.castling[1]) {
            part += "Q";
        }
        if (npos.castling[2]) {
            part += "k";
        }
        if (npos.castling[3]) {
            part += "q";
        }
        if (part == "") {
            part = "-";
        }
        fen += " " + part;
    }

    // Part 4 -- En passant square
    if (pos.ep) {
        const auto sq = lsbll(pos.ep);

        fen += " ";
        fen += static_cast<char>('a' + sq % 8);
        if (pos.flipped) {
            fen += '3';
        } else {
            fen += '6';
        }
    } else {
        fen += " -";
    }

    // Part 5 -- Halfmove clock
    fen += " " + std::to_string(pos.halfmoves);

    /*
    // Part 6 -- Fullmove number
    fen += " " + std::to_string(fullmove_clock_);
    */

    return fen;
}

}  // namespace chess

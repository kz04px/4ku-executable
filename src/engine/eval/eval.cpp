#include "eval.hpp"
#include <chess/position.hpp>

namespace eval {

constexpr const int piece_values[] = {100, 300, 325, 500, 900, 100000};

int eval(const chess::Position &pos) {
    int score = 0;

    score += piece_values[0] * (pos.colour[0] & pos.pieces[0]).count();
    score -= piece_values[0] * (pos.colour[1] & pos.pieces[0]).count();

    score += piece_values[1] * (pos.colour[0] & pos.pieces[1]).count();
    score -= piece_values[1] * (pos.colour[1] & pos.pieces[1]).count();

    score += piece_values[2] * (pos.colour[0] & pos.pieces[2]).count();
    score -= piece_values[2] * (pos.colour[1] & pos.pieces[2]).count();

    score += piece_values[3] * (pos.colour[0] & pos.pieces[3]).count();
    score -= piece_values[3] * (pos.colour[1] & pos.pieces[3]).count();

    score += piece_values[4] * (pos.colour[0] & pos.pieces[4]).count();
    score -= piece_values[4] * (pos.colour[1] & pos.pieces[4]).count();

    return score;
}

}  // namespace eval

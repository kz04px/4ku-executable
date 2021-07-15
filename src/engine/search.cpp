#include "search.hpp"
#include <chess/attack.hpp>
#include <chess/bitboard.hpp>
#include <chess/makemove.hpp>
#include <chess/move.hpp>
#include <chess/movegen.hpp>
#include <chess/position.hpp>

namespace search {

const int material[] = {100, 300, 325, 500, 900};

[[nodiscard]] int eval(const chess::Position &pos) {
    int score = 0;

    for (int c = 0; c < 2; ++c) {
        // Material
        for (int p = 0; p < 5; ++p) {
            score += material[p] * chess::count(pos.pieces[p] & pos.colour[c]);
        }

        score = -score;
    }

    return score;
}

[[nodiscard]] chess::Move dumb(const chess::Position &pos) {
    chess::Move moves[256];
    const int num_moves = chess::movegen(pos, moves);
    int best_score = -9999999;
    int best_move_idx = 0;

    for (int i = 0; i < num_moves; ++i) {
        auto npos = pos;

        // Check move legality
        if (!chess::makemove(npos, moves[i])) {
            continue;
        }

        const auto score = -eval(npos);

        if (score > best_score) {
            best_score = score;
            best_move_idx = i;
        }
    }

    return moves[best_move_idx];
}

}  // namespace search

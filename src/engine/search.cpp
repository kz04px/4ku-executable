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
        for (int p = 0; p < 5; ++p) {
            score += material[p] * chess::count(pos.colour[c] & pos.pieces[p]);
        }

        score = -score;
    }

    return score;
}

[[nodiscard]] int search(const chess::Position &pos,
                         int alpha,
                         const int beta,
                         const int depth,
                         chess::Move &pv,
                         const timepoint stop_time) {
    if (depth == 0) {
        return eval(pos);
    }

    // Did we run out of time?
    if (now() >= stop_time) {
        return 0;
    }

    chess::Move moves[256];
    const int num_moves = chess::movegen(pos, moves);
    int best_score = -inf;
    int best_idx = -1;

    for (int i = 0; i < num_moves; ++i) {
        auto npos = pos;

        // Check move legality
        if (!chess::makemove(npos, moves[i])) {
            continue;
        }

        const auto score = -search(npos, -beta, -alpha, depth - 1, pv, stop_time);

        if (score > best_score) {
            best_idx = i;
            best_score = score;

            if (score > alpha) {
                alpha = score;
            }
        }

        if (alpha >= beta) {
            break;
        }
    }

    // No legal moves
    if (best_idx == -1) {
        const auto ksq = chess::lsbll(pos.colour[0] & pos.pieces[static_cast<int>(chess::Piece::King)]);
        const auto in_check = chess::attacked(pos, ksq, true);

        // Checkmate
        if (in_check) {
            return -mate_score;
        }
        // Stalemate
        else {
            return 0;
        }
    }

    pv = moves[best_idx];

    return best_score;
}

}  // namespace search

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

int alphabeta(const chess::Position &pos,
              int alpha,
              const int beta,
              int depth,
              const int ply,
              const int stop_time,
              chess::Move *pvline) {
    const int ksq = chess::lsbll(pos.colour[0] & pos.pieces[static_cast<int>(chess::Piece::King)]);
    const auto in_check = chess::attacked(pos, ksq, true);

    if (depth <= 0) {
        const auto static_eval = eval(pos);
        if (static_eval >= beta) {
            return beta;
        }

        if (alpha < static_eval) {
            alpha = static_eval;
        }
    }

    // Did we run out of time?
    if (clock() >= stop_time) {
        return 0;
    }

    chess::Move moves[256];
    int num_moves = 0;
    chess::movegen(pos, moves, num_moves, true);
    if (depth > 0) {
        chess::movegen(pos, moves, num_moves, false);
    }

    for (int i = 0; i < num_moves; ++i) {
        if (moves[i] == pvline[ply]) {
            moves[i] = moves[0];
            moves[0] = pvline[ply];
            break;
        }
    }

    int best_score = -INF;
    for (int i = 0; i < num_moves; ++i) {
        auto npos = pos;

        // Check move legality
        if (!chess::makemove(npos, moves[i])) {
            continue;
        }

        const auto score = -alphabeta(npos, -beta, -alpha, depth - 1, ply + 1, stop_time, pvline);

        if (score > best_score) {
            best_score = score;

            if (score > alpha) {
                alpha = score;
                pvline[ply] = moves[i];
            }
        }

        if (alpha >= beta) {
            break;
        }
    }

    // No legal moves
    if (depth > 0 && best_score == -INF) {
        // Checkmate
        if (in_check) {
            return -MATE_SCORE;
        }
        // Stalemate
        else {
            return 0;
        }
    }

    return best_score;
}

}  // namespace search

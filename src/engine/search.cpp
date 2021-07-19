#include "search.hpp"
#include "time.hpp"
#include <chess/attack.hpp>
#include <chess/bitboard.hpp>
#include <chess/makemove.hpp>
#include <chess/move.hpp>
#include <chess/movegen.hpp>
#include <chess/position.hpp>

namespace search {

const int material[] = {100, 300, 325, 500, 900, 0};

[[nodiscard]] int eval(const chess::Position &pos) {
    int score = 0;

    for (int c = 0; c < 2; ++c) {
        for (int p = 0; p < 6; ++p) {
            auto copy = pos.colour[c] & pos.pieces[p];
            while (copy) {
                const auto sq = chess::lsbll(copy);
                copy &= copy - 1;

                // PST
                const int rank = sq >> 3;
                const int file = sq & 7;
                const int center_tropism = -std::abs(7 - rank - file) - std::abs(rank - file);
                score += center_tropism * (6 - p);

                // Material
                score += material[p];
            }
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
              const long long int stop_time,
              chess::Move *pvline) {
    const int ksq = chess::lsbll(pos.colour[0] & pos.pieces[static_cast<int>(chess::Piece::King)]);
    const auto in_check = chess::attacked(pos, ksq, true);

    if (depth == 0) {
        return eval(pos);
    }

    // Did we run out of time?
    if (now() >= stop_time) {
        return 0;
    }

    chess::Move moves[256];
    const int num_moves = chess::movegen(pos, moves);
    int best_score = -INF;

    for (int i = 0; i < num_moves; ++i) {
        if (moves[i] == pvline[ply]) {
            moves[i] = moves[0];
            moves[0] = pvline[ply];
            break;
        }
    }

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
    if (best_score == -INF) {
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

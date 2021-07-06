#include "alphabeta.hpp"
#include <chess/attack.hpp>
#include <chess/makemove.hpp>
#include <chess/move.hpp>
#include <chess/movegen.hpp>
#include <chess/position.hpp>
#include "../eval/eval.hpp"

namespace search {

[[nodiscard]] int alphabeta(const chess::Position &pos, int alpha, int beta, int depth, chess::Move &pv) {
    if (depth == 0) {
        return eval::eval(pos);
    }

    chess::Move moves[256];
    const int num_moves = chess::movegen(pos, moves);
    int best_score = -1000000;

    // Move ordering

    for (int i = 0; i < num_moves; ++i) {
        auto npos = pos;

        chess::makemove(npos, moves[i]);

        const int ksq = (npos.colour[1] & npos.pieces[static_cast<int>(chess::Piece::King)]).lsbll();

        if (chess::attacked(npos, ksq, false)) {
            continue;
        }

        const auto score = -alphabeta(npos, -beta, -alpha, depth - 1, pv);

        if (score > best_score) {
            best_score = score;
            pv = moves[i];
        }

        if (alpha >= beta) {
            break;
        }
    }

    return best_score;
}

}  // namespace search

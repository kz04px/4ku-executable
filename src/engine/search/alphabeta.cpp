#include "alphabeta.hpp"
#include <chess/makemove.hpp>
#include <chess/move.hpp>
#include <chess/movegen.hpp>
#include <chess/position.hpp>
#include <chess/undomove.hpp>
#include "../eval/eval.hpp"

namespace search {

[[nodiscard]] int alphabeta(chess::Position &pos, int alpha, int beta, int depth) {
    if (depth == 0) {
        return eval::eval(pos);
    }

    chess::Move moves[256];
    const int num_moves = chess::movegen(pos, moves);
    int best_score = -1000000;  // std::numeric_limits<int>::lowest();

    // Move ordering

    for (int i = 0; i < num_moves; ++i) {
        chess::makemove(pos, moves[i]);
        const auto score = -alphabeta(pos, -beta, -alpha, depth - 1);
        chess::undomove(pos, moves[i]);

        if (score > best_score) {
            best_score = score;
        }

        if (alpha >= beta) {
            break;
        }
    }

    return best_score;
}

}  // namespace search

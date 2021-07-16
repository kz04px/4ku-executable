#include "search.hpp"
#include <chess/attack.hpp>
#include <chess/bitboard.hpp>
#include <chess/makemove.hpp>
#include <chess/move.hpp>
#include <chess/movegen.hpp>
#include <chess/position.hpp>

namespace search {

[[nodiscard]] chess::Move dumb(const chess::Position &pos) {
    chess::Move moves[256];
    const int num_moves = chess::movegen(pos, moves);

    for (int i = 0; i < num_moves; ++i) {
        auto npos = pos;

        // Check move legality
        if (!chess::makemove(npos, moves[i])) {
            continue;
        }

        return moves[i];
    }

    return {};
}

}  // namespace search

#include "perft.hpp"
#include "attack.hpp"
#include "makemove.hpp"
#include "move.hpp"
#include "movegen.hpp"
#include "position.hpp"

namespace chess {

std::uint64_t perft(const Position &pos, const int depth) {
    if (depth == 0) {
        return 1;
    }

    std::uint64_t nodes = 0;
    Move moves[256];
    int num_moves = 0;
    movegen(pos, moves, num_moves, true);
    movegen(pos, moves, num_moves, false);

    for (int i = 0; i < num_moves; ++i) {
        auto npos = pos;

        // Check move legality
        if (!chess::makemove(npos, moves[i])) {
            continue;
        }

        nodes += perft(npos, depth - 1);
    }

    return nodes;
}

}  // namespace chess

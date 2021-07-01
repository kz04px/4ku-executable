#include "perft.hpp"
#include "attack.hpp"
#include "makemove.hpp"
#include "move.hpp"
#include "movegen.hpp"
#include "position.hpp"
#include "undomove.hpp"

namespace chess {

std::uint64_t perft(Position &pos, const int depth) {
    if (depth == 0) {
        return 1;
    }

    std::uint64_t nodes = 0;
    Move moves[256];
    const int num_moves = movegen(pos, moves);

    for (int i = 0; i < num_moves; ++i) {
        makemove(pos, moves[i]);

        const int ksq = (pos.colour[1] & pos.pieces[static_cast<int>(chess::Piece::King)]).lsbll();

        if (chess::attacked(pos, ksq, false)) {
            undomove(pos, moves[i]);
            continue;
        }

        nodes += perft(pos, depth - 1);
        undomove(pos, moves[i]);
    }

    return nodes;
}

}  // namespace chess

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
    const int num_moves = movegen(pos, moves);

    for (int i = 0; i < num_moves; ++i) {
        auto npos = pos;

        makemove(npos, moves[i]);

        const int ksq = (npos.colour[1] & npos.pieces[static_cast<int>(chess::Piece::King)]).lsbll();

        if (chess::attacked(npos, ksq, false)) {
            continue;
        }

        nodes += perft(npos, depth - 1);
    }

    return nodes;
}

}  // namespace chess

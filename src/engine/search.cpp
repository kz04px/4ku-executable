#include "search.hpp"
#include <chess/attack.hpp>
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

        chess::makemove(npos, moves[i]);

        const int ksq = (npos.colour[1] & npos.pieces[static_cast<int>(chess::Piece::King)]).lsbll();

        if (chess::attacked(npos, ksq, false)) {
            continue;
        }

        return moves[i];
    }

    return {};
}

}  // namespace search

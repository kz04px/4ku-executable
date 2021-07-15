#include <stdio.h>
#include <chess/move.hpp>
#include <chess/position.hpp>
#include "../search.hpp"
#include "listen.hpp"

namespace uci {

void go(chess::Position &pos, const int time) {
    char bestmove_str[] = "bestmove 123456";

    const auto stop_time = clock() + time / 30'000.0f * CLOCKS_PER_SEC;

    for (int i = 1; i < 128; ++i) {
        chess::Move bestmove;
        search::negamax(pos, -INF, INF, i, bestmove, stop_time);

        // Did we run out of time?
        if (clock() >= stop_time) {
            break;
        }

        chess::move_str(bestmove, &bestmove_str[9], pos.flipped);
    }

    puts(bestmove_str);
}

}  // namespace uci

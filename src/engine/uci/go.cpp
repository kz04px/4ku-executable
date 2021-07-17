#include <stdio.h>
#include <chess/move.hpp>
#include <chess/position.hpp>
#include "../search.hpp"
#include "listen.hpp"

namespace uci {

void go(chess::Position &pos, const int time) {
    char bestmove_str[] = "bestmove 123456";

    const auto stop_time = clock() + time / 30'000.0f * CLOCKS_PER_SEC;
    chess::Move pvline[128];
    for (int i = 1; i < 128; ++i) {
        search::alphabeta(pos, -INF, INF, i, 0, stop_time, pvline);

        // Did we run out of time?
        if (clock() >= stop_time) {
            break;
        }

        chess::move_str(pvline[0], &bestmove_str[9], pos.flipped);
    }

    puts(bestmove_str);
}

}  // namespace uci

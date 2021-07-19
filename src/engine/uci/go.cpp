#include <stdio.h>
#include <chess/move.hpp>
#include <chess/position.hpp>
#include <chrono>
#include "../search.hpp"
#include "listen.hpp"

namespace uci {

void go(chess::Position &pos, const int time) {
    const auto stop_time = std::chrono::steady_clock::now() + std::chrono::milliseconds(time / 32);
    char bestmove_str[] = "bestmove 123456";
    chess::Move pvline[128];

    // Iterative deepening
    for (int i = 1; i < 128; ++i) {
        search::alphabeta(pos, -INF, INF, i, 0, stop_time, pvline);

        // Did we run out of time?
        if (std::chrono::steady_clock::now() >= stop_time) {
            break;
        }

        chess::move_str(pvline[0], &bestmove_str[9], pos.flipped);
    }

    puts(bestmove_str);
}

}  // namespace uci

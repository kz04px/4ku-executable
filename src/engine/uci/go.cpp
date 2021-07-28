#include <stdio.h>
#include <chess/move.hpp>
#include <chess/position.hpp>
#include "../search.hpp"
#include "../time.hpp"
#include "listen.hpp"

namespace uci {

void go(chess::Position &pos, const int time) {
    const auto stop_time = now() + time / 30;
    char bestmove_str[] = "bestmove       ";
    chess::Move pvline[128];
#ifdef USE_SEARCHINFO
    search::nodes_searched = 0;
    const auto start_time = now();
#endif
    // Iterative deepening
    for (int i = 1; i < 128; ++i) {
#ifdef USE_SEARCHINFO
        const auto score = search::alphabeta(pos, -INF, INF, i, 0, stop_time, pvline);
#else
        search::alphabeta(pos, -INF, INF, i, 0, stop_time, pvline);
#endif

        // Did we run out of time?
        if (now() >= stop_time) {
            break;
        }

        chess::move_str(pvline[0], &bestmove_str[9], pos.flipped);
#ifdef USE_SEARCHINFO
        auto elapsed = now() - start_time;
        if (elapsed == 0) {
            elapsed = 1;
        }
        const auto nps = search::nodes_searched * 1000 / elapsed;
        std::printf("info depth %i score %i nodes %lld nps %lld time %lld\n", i, score, search::nodes_searched, nps, elapsed);
#endif
    }

    puts(bestmove_str);
}

}  // namespace uci

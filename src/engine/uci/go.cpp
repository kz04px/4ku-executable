#include <stdio.h>
#include <chess/move.hpp>
#include <chess/position.hpp>
#include "../search.hpp"
#include "listen.hpp"

namespace uci {

void go(chess::Position &pos, const int time) {
    const auto stop_time = now() + time / 30;
    search::StackData ss[128];
    char bestmove_str[] = "bestmove 123456";
    chess::Move bestmove;

    for (int i = 0; i < 128; ++i) {
        search::search(pos, -search::inf, search::inf, i, ss, stop_time);

        // Did we run out of time?
        if (now() >= stop_time) {
            break;
        }

        chess::move_str(pos, ss->pv, &bestmove_str[9]);
    }

    puts(bestmove_str);
}

}  // namespace uci

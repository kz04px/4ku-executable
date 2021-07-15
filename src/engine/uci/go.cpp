#include <stdio.h>
#include <chess/move.hpp>
#include <chess/position.hpp>
#include "../search.hpp"
#include "listen.hpp"

namespace uci {

void go(chess::Position &pos) {
    const auto bestmove = search::dumb(pos);
    char bestmove_str[] = "bestmove 123456";
    chess::move_str(bestmove, &bestmove_str[9], pos.flipped);
    puts(bestmove_str);
}

}  // namespace uci

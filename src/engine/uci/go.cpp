#include <stdio.h>
#include <chess/move.hpp>
#include <chess/position.hpp>
#include "../search.hpp"
#include "listen.hpp"

namespace uci {

void go(chess::Position &pos) {
    const auto bestmove = search::dumb(pos);
    char movestr[6];
    chess::move_str(bestmove, movestr, pos.flipped);
    printf("bestmove %s\n", movestr);
}

}  // namespace uci

#include <stdio.h>
#include <chess/position.hpp>
#include "../search/alphabeta.hpp"
#include "listen.hpp"

namespace uci {

void go(chess::Position &pos) {
    const int score = search::alphabeta(pos, -10000, 10000, 1);
    printf("info score cp %i\n", score);
    printf("bestmove 0000\n");
}

}  // namespace uci

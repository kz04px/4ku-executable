#include <stdio.h>
#include <chess/move.hpp>
#include <chess/position.hpp>
#include "../search/alphabeta.hpp"
#include "listen.hpp"

namespace uci {

constexpr char promos[] = {'p', 'n', 'b', 'r', 'q', 'k'};

void go(chess::Position &pos) {
    chess::Move bestmove;
    const int score = search::alphabeta(pos, -10000, 10000, 1, bestmove);
    printf("info score cp %i\n", score);

    char movestr[5] = {};
    chess::move_str(bestmove, movestr, pos.flipped);
    printf("bestmove %s\n", movestr);
}

}  // namespace uci

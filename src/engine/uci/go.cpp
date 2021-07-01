#include <stdio.h>
#include <chess/move.hpp>
#include <chess/position.hpp>
#include "../search/alphabeta.hpp"
#include "listen.hpp"

namespace uci {

constexpr char promos[] = {'p', 'n', 'b', 'r', 'q', 'k'};

void move_str(const chess::Move &move, char *str) {
    str[0] = static_cast<char>((move.from % 8) + 'a');
    str[1] = static_cast<char>((move.from / 8) + '1');
    str[2] = static_cast<char>((move.to % 8) + 'a');
    str[3] = static_cast<char>((move.to / 8) + '1');

    if (move.type == chess::Move::Type::Promo || move.type == chess::Move::Type::Promocapture) {
        str[4] = promos[static_cast<int>(move.promo)];
    }
}

void go(chess::Position &pos) {
    chess::Move bestmove;
    const int score = search::alphabeta(pos, -10000, 10000, 1, bestmove);
    printf("info score cp %i\n", score);

    char movestr[5] = {};
    move_str(bestmove, movestr);
    printf("bestmove %s\n", movestr);
}

}  // namespace uci

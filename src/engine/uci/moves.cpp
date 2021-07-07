#include <stdio.h>
#include <string.h>
#include <chess/makemove.hpp>
#include <chess/move.hpp>
#include <chess/movegen.hpp>
#include <chess/position.hpp>
#include "listen.hpp"

namespace uci {

void moves(chess::Position &pos) {
    char word[4096];
    chess::Move moves[256];
    char movestr[6];
    bool quit = false;

    while (!quit) {
        int word_size = 0;
        char c;
        while ((c = getchar())) {
            if (c == '\n' || c == '\0') {
                quit = true;
                break;
            } else if (c == ' ') {
                break;
            }
            word[word_size] = c;
            word_size++;
        }
        word[word_size] = '\0';

        const int num_moves = chess::movegen(pos, moves);

        for (int i = 0; i < num_moves; ++i) {
            chess::move_str(moves[i], movestr, pos.flipped);
            if (strncmp(movestr, word, strlen(movestr)) == 0) {
                chess::makemove(pos, moves[i]);
                break;
            }
        }
    }
}

}  // namespace uci

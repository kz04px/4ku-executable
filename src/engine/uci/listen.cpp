#include "listen.hpp"
#include <stdio.h>
#include <string.h>
#include <chess/makemove.hpp>
#include <chess/move.hpp>
#include <chess/movegen.hpp>
#include <chess/position.hpp>

namespace uci {

void listen() {
    char word[4096];
    auto pos = chess::Position();
    chess::Move moves[256];

    // Pretend to wait for "uci"
    getchar();

    // Tell the GUI about us
    puts("id name 4ku\nid author kz04px\nuciok");

    while (true) {
        // Get next word
        char *ptr = word;
        char c;
        while ((c = getchar())) {
            if (c == '\n' || c == '\0' || c == ' ') {
                break;
            }
            *ptr = c;
            ptr++;
        }
        *ptr = '\0';

        // quit
        if (word[0] == 'q') {
            break;
        }
        // isready
        else if (word[0] == 'i' && word[1] == 's') {
            isready();
        }
        // go
        else if (word[0] == 'g' && word[1] == 'o') {
            go(pos);
        }
        // position
        else if (word[0] == 'p' && word[1] == 'o') {
            pos = chess::Position();
        }
        // Try parse a move
        else {
            const int num_moves = chess::movegen(pos, moves);
            for (int i = 0; i < num_moves; ++i) {
                char movestr[6];
                chess::move_str(pos, moves[i], movestr);
                if (strncmp(movestr, word, strlen(movestr)) == 0) {
                    chess::makemove(pos, moves[i]);
                    break;
                }
            }
        }
    }
}

}  // namespace uci

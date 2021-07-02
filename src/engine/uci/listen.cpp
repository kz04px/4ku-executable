#include "listen.hpp"
#include <stdio.h>
#include <string.h>
#include <chess/position.hpp>

namespace uci {

void listen() {
    auto pos = chess::Position();
    char word[4096];

    // Pretend to wait for "uci"
    getchar();

    // Tell the GUI about us
    puts("id name 4ku\nid author kz04px\nuciok");

    while (scanf("%s", word) != EOF) {
        // quit
        if (strncmp(word, "quit", 4) == 0) {
            break;
        }
        // isready
        else if (strncmp(word, "isready", 7) == 0) {
            isready();
        }
        // go
        else if (strncmp(word, "go", 2) == 0) {
            go(pos);
        }
        // moves
        else if (strncmp(word, "moves", 5) == 0) {
            pos = chess::Position();
            moves(pos);
        }
    }
}

}  // namespace uci

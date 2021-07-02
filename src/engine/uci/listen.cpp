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
    getchar();
    getchar();

    // Tell the GUI about us
    puts("id name 4ku\nid author kz04px\nuciok");

    char c;
    while ((c = getchar()) != EOF) {
        switch (c) {
            // quit
            case 'q':
                return;
            // isready
            case 'i':
                getchar();  // s
                getchar();  // r
                getchar();  // e
                getchar();  // a
                getchar();  // d
                getchar();  // y
                isready();
                break;
            // go
            case 'g':
                getchar();  // o
                go(pos);
                break;
            // moves
            case 'm':
                getchar();  // o
                getchar();  // v
                getchar();  // e
                getchar();  // s
                pos = chess::Position();
                moves(pos);
                break;
        }
    }
}

}  // namespace uci

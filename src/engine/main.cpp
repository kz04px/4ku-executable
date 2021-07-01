#include <stdio.h>
#include "uci/listen.hpp"

int main() {
    setbuf(stdout, NULL);
    uci::listen();
    return 0;
}

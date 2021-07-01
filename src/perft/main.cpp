#include <chess/fen.hpp>
#include <chess/perft.hpp>
#include <chess/position.hpp>
#include <chrono>
#include <iostream>

int main() {
    auto pos = chess::Position();
    chess::set_fen(pos, "startpos");

    std::cout << pos;
    std::cout << chess::get_fen(pos) << "\n\n";

    for (int i = 1; i <= 6; ++i) {
        const auto t0 = std::chrono::steady_clock::now();
        const auto nodes = chess::perft(pos, i);
        const auto t1 = std::chrono::steady_clock::now();
        const auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);
        const auto nps = dt.count() ? (1000 * nodes) / dt.count() : 0;

        std::cout << "info";
        std::cout << " depth " << i;
        std::cout << " nodes " << nodes;
        std::cout << " time " << dt.count();
        if (nps > 0) {
            std::cout << " nps " << nps;
        }
        std::cout << std::endl;
    }

    return 0;
}

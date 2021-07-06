#ifndef PERFT_HPP
#define PERFT_HPP

#include <cstdint>

namespace chess {

struct Position;

std::uint64_t perft(const chess::Position &pos, const int depth);

}  // namespace chess

#endif

#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <chess/move.hpp>

namespace chess {
class Position;
}  // namespace chess

namespace search {

[[nodiscard]] chess::Move dumb(const chess::Position &pos);

}  // namespace search

#endif

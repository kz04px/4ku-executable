#ifndef EVAL_EVAL_HPP
#define EVAL_EVAL_HPP

namespace chess {
struct Position;
}  // namespace chess

namespace eval {

[[nodiscard]] int eval(const chess::Position &pos);

}

#endif

#ifndef UCI_LISTEN_HPP
#define UCI_LISTEN_HPP

namespace chess {
class Position;
}  // namespace chess

namespace uci {

void listen();
void go(chess::Position &pos);

}  // namespace uci

#endif

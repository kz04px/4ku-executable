#ifndef UCI_LISTEN_HPP
#define UCI_LISTEN_HPP

namespace chess {
class Position;
}  // namespace chess

namespace uci {

void listen();
void isready();
void moves(chess::Position &pos);
void go(chess::Position &pos);

}  // namespace uci

#endif

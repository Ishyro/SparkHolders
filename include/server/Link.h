#ifndef _LINK_SERVER_H_
#define _LINK_SERVER_H_

#include "Values.h"

class Link {
  public:
    Link(Socket s, Adventure * adventure):s(s),adventure(adventure) {
      closed = false;
      initialize();
    }
    void initialize();
    void sendState();
    Action * receiveAction();
    bool isClosed();
    void markClosed();
    Character * getPlayer();
    void changeSocket(Socket s);
  private:
    Adventure * adventure;
    Character * player;
    Socket s;
    bool closed;
};

#endif // _LINK_SERVER_H_

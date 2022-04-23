#ifndef _LINK_H_
#define _LINK_H_

#include "Values.h"

class Link {
  public:
    Link(Socket s, Adventure * adventure):s(s),adventure(adventure) {
      closed = false;
      playerChoices();
    }
    void playerChoices();
    void sendMap();
    Action * receiveAction();
    bool isClosed();
    void markClosed();
    Character * getPlayer();
  private:
    Adventure * adventure;
    Character * player;
    Socket s;
    bool closed;
};

#endif // _LINK_H_

#ifndef _LINK_H_
#define _LINK_H_

#include "Values.h"

class Link {
  public:
    Link(Socket s, Adventure * adventure):s(s),adventure(adventure) {
      playerChoices();
    }
    void playerChoices();
    void sendMap();
    Action * receiveAction();
  private:
    Adventure * adventure;
    Character * player;
    Socket s;
};

#endif // _LINK_H_

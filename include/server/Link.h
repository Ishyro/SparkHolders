#ifndef _LINK_SERVER_H_
#define _LINK_SERVER_H_

#include <map>

#include "Values.h"

class Link {
  public:
    Link(
      Adventure * adventure
    ):
      adventure(adventure),
      closed(false),
      ready(false)
    {}
    void initialize(Socket s);
    void listen();
    void sendState();
    void sendStart();
    bool isClosed();
    bool isReady();
    bool isMaster();
    void markClosed();
    Character * getCharacter(long id);
    void changeSocket(Socket s);
    bool hasActions();
    std::list<Action *> getActions();
    bool getNeedToUpdateActions();
    std::string getUsername();
  private:
    Adventure * adventure;
    std::map<const long, Character *> characters;
    std::list<Action *> actions;
    Socket s;
    bool closed;
    bool ready;
    bool master;
    std::string username;
    std::string password;
};

#endif // _LINK_SERVER_H_

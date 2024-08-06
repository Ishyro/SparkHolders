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
      shuting_down(false),
      ready(false)
    {}
    // non copyable because of mutex
    Link(const Link&) = delete;
    Link & operator=(const Link&) = delete;
    //
    void initialize(Socket s);
    void listen();
    void sendState();
    void sendStart();
    bool isClosed();
    bool isShutingDown();
    bool isReady();
    bool isMaster();
    void markClosed();
    Character * getCharacter();
    void changeSocket(Socket s);
    bool hasActions();
    std::list<Action *> getAction();
    bool getNeedToUpdateActions();
    std::string getUsername();
  private:
    Adventure * adventure;
    Character * character = nullptr;
    Action * action = nullptr;
    Socket s;
    bool closed;
    bool shuting_down;
    bool ready;
    bool master;
    std::string password;
    std::mutex mutex;
};

#endif // _LINK_SERVER_H_

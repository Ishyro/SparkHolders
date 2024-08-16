#ifndef _LINK_CLIENT_H_
#define _LINK_CLIENT_H_

#include <vector>
#include <map>
#include <list>

#include "clients/EnglishKeyHolder.h"

#include "Values.h"

class Link {
  public:
    Link(Socket s):s(s) {}
    void initialize(std::string password);
    void listen();
    void sendChoices(std::string name, std::string attributes, std::string race, std::string origin, std::string culture, std::string religion, std::string profession);
    void sendAction(int32_t type, void * arg1, void * arg2, int32_t mana_cost);
    void receiveState(std::string msg);
    void sendReady();
    void sendPause();
    void sendUnpause();
    int64_t getTickRate();
    std::vector<Attributes *> getStartingAttributes();
    std::vector<Way *> getStartingWays();
    std::list<std::pair<const std::string, const std::string>> getWaysIncompatibilities();
    Character * getPlayer();
    Adventure * getAdventure();
    bool hasState();
    StateDisplay * getState();
    std::string getEnglishFromKey(std::string key);
    bool isClosed();
    bool isReady();
    bool isStarted();
    void close(bool shutdown);
  private:
    std::vector<Attributes *> * attributes;
    std::vector<Way *> * ways;
    std::list<std::pair<const std::string, const std::string>> * waysIncompatibilities;
    Character * character = nullptr;
    std::list<StateDisplay *> state_pile = std::list<StateDisplay *>();
    Adventure * adventure;
    bool ready = false;
    bool master;
    bool closed = false;
    bool started = false;
    int64_t tickrate;
    Socket s;
    EnglishKeyHolder * key_holder;
    std::string language;
};

#endif // _LINK_CLIENT_H_

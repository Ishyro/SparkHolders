#ifndef _LINK_CLIENT_H_
#define _LINK_CLIENT_H_

#include <vector>
#include <map>
#include <list>

#include "clients/Translator.h"

#include "Values.h"

class Link {
  public:
    Link(Socket s):s(s) {}
    Translator * initialize(std::string language);
    void sendChoices(std::string name, std::string attributes, std::string race, std::string origin, std::string culture, std::string religion, std::string profession);
    StateDisplay * receiveState();
    void sendAction(int type, void * arg1, void * arg2, int overcharge_power, int overcharge_duration, int overcharge_rang);
    std::vector<Attributes *> getStartingAttributes();
    std::vector<Way *> getStartingWays();
    std::list<std::pair<const std::string, const std::string>> getWaysIncompatibilities();
    Character * getPlayer();
    Adventure * getAdventure();
    bool getNeedToUpdateActions();
  private:
    std::vector<Attributes *> * attributes;
    std::vector<Way *> * ways;
    std::list<std::pair<const std::string, const std::string>> * waysIncompatibilities;
    Character * player;
    Adventure * adventure;
    bool need_to_update_actions = true;
    Socket s;
};

#endif // _LINK_CLIENT_H_

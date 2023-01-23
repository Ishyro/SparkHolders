#ifndef _LINK_CLIENT_H_
#define _LINK_CLIENT_H_

#include <vector>
#include <map>
#include <list>

#include "Values.h"

class Link {
  public:
    Link(Socket s):s(s) {}
    void loadChoices();
    std::list<std::string> receiveTranslationPaths();
    void sendChoices(std::string name, std::string attributes, std::string race, std::string origin, std::string culture, std::string religion, std::string profession);
    MapDisplay * receiveMap();
    void sendAction(int type, float orientation, Skill * skill, int target_id, int target_x, int target_y, std::string object, int overcharge_power, int overcharge_duration, int overcharge_range);
    std::vector<Attributes *> getStartingAttributes();
    std::vector<Way *> getStartingWays();
    std::list<std::pair<const std::string, const std::string>> getWaysIncompatibilities();
    Character * getPlayer();
  private:
    std::vector<Attributes *> * attributes;
    std::vector<Way *> * ways;
    std::list<std::pair<const std::string, const std::string>> * waysIncompatibilities;
    Character * player;
    long serverCharacterId;
    Socket s;
};

#endif // _LINK_CLIENT_H_

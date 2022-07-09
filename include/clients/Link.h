#ifndef _LINK_H_
#define _LINK_H_

#include <vector>
#include <map>
#include <list>

#include "Values.h"

class Link {
  public:
    Link(Socket s):s(s) {}
    void loadChoices();
    std::list<std::string> receiveTraductionPaths();
    void sendChoices(std::string name, std::string attibutes, std::string race, std::string origin, std::string culture, std::string religion, std::string profession);
    MapDisplay * receiveMap();
    void sendAction(int type, int orientation, Skill * skill, int target_id, int target_x, int target_y, std::string object, int overcharge);
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

#endif // _LINK_H_

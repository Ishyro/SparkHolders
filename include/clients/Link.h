#ifndef _LINK_H_
#define _LINK_H_

#include <list>

#include "Values.h"

class Link {
  public:
    Link(Socket s, Adventure * adventure):s(s),adventure(adventure) {
      loadChoices();
    }
    void loadChoices();
    void sendChoices(std::string name, std::string attibutes, std::string race, std::string origin, std::string culture, std::string religion, std::string profession);
    MapDisplay * receiveMap();
    void sendAction(int type, int orientation, ProjectileDisplay * projectile, Skill * skill, CharacterDisplay * target, Item * item, Weapon * weapon);
    std::list<Attributes *> getStartingAttributes();
    std::list<Way *> getStartingWays();
  private:
    std::list<Attributes *> * attributes;
    std::list<Way *> * ways;
    Adventure * adventure;
    Socket s;
};

#endif // _LINK_H_

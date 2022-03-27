#ifndef _QUEST_H_
#define _QUEST_H_

#include <string>
#include <list>

#include "data/Character.h"
#include "data/Map.h"
#include "data/Item.h"
#include "data/Weapon.h"
#include "data/Adventure.h"

#include "utils/FileOpener.h"

typedef struct Step {
  const int type;
  const Character * target;
  const Item * goal_item;
  const Weapon * goal_weapon;
  const int disover_map_id;
  const Speech * todo;
  const Speech * when_done;
} Step;

class Quest {
  public:
    const std::string name;
    const bool holdEvent;
    std::list<Item *> items_rewards;
    std::list<Weapon *> weapons_rewards;
    Quest(std::string name) {
      //TODO INI_READER
    }
    void getCurrentStep();
    bool stepDone(Adventure * adventure);
    bool questDone();
  private:
    std::list<Step *> steps;
    Event * event;
};

#endif // _QUEST_H_

#ifndef _QUEST_H_
#define _QUEST_H_

#include <string>
#include <list>

#include "src/data/Character.h"
#include "src/data/Map.h"
#include "src/data/Item.h"
#include "src/data/Weapon.h"
#include "src/data/Adventure.h"

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
      //TODO XML_READER
    }
    void getCurrentStep();
    bool stepDone(Adventure * adventure);
    bool questDone();
  private:
    std::list<Step *> steps;
    Event * event;
};

#endif // _QUEST_H_

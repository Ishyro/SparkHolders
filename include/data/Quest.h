#ifndef _QUEST_H_
#define _QUEST_H_

#include <string>
#include <list>

typedef struct Step {
  const int type;
  const Character * target;
  const Item * goal_item;
  const Weapon * goal_weapon;
  const int disover_map_id;
  const Speech * todo;
  const Speech * when_done;
} Step;

namespace quest {
  static long id_cpt = 0;
}

class Quest {
  public:
    const long id = ++quest::id_cpt;
    const std::string name;
    const bool holdEvent;
    std::list<Item *> items_rewards;
    std::list<Weapon *> weapons_rewards;
    Quest(
      std::string name,
      bool holdEvent,
      std::list<Item *> items_rewards,
      std::list<Weapon *> weapons_rewards,
      std::list<Step *> steps,
      Event * event
    ):
      name(name),
      holdEvent(holdEvent),
      items_rewards(items_rewards),
      weapons_rewards(weapons_rewards),
      steps(steps),
      event(event)
    {}
    Quest(const Quest * quest):
      name(quest->name),
      holdEvent(quest->holdEvent),
      items_rewards(quest->items_rewards),
      weapons_rewards(quest->weapons_rewards),
      steps(quest->steps),
      event(quest->event)
    {}
    void getCurrentStep();
    bool stepDone(Adventure * adventure);
    bool questDone();
    bool operator == (const Quest& q) const { return id == q.id; }
    bool operator != (const Quest& q) const { return !operator==(q); }
  private:
    std::list<Step *> steps;
    Event * event;
};

#endif // _QUEST_H_

#ifndef _QUEST_H_
#define _QUEST_H_

#include <string>
#include <list>

struct Step {
  const int32_t type;
  const Character * target;
  const Item * goal_item;
  const int32_t discover_map_id;
  Speech * todo;
  Speech * when_done;
};

namespace quest {
  static int64_t id_cpt = 0;
}

class Quest {
  public:
    const int64_t id = ++quest::id_cpt;
    const std::string name;
    const bool holdEvent;
    std::list<Item *> items_rewards;
    Quest(
      std::string name,
      bool holdEvent,
      std::list<Item *> items_rewards,
      std::list<Step *> steps,
      Event * event
    ):
      name(name),
      holdEvent(holdEvent),
      items_rewards(items_rewards),
      steps(steps),
      event(event)
    {}
    Quest(const Quest * quest):
      name(quest->name),
      holdEvent(quest->holdEvent),
      items_rewards(quest->items_rewards),
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

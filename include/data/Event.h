#ifndef _EVENT_H_
#define _EVENT_H_

#include <list>
#include <string>

#include "Values.h"

namespace event {
  static int64_t id_counter = 0;
}


class Event {
  public:
    const int64_t id = ++event::id_counter;
    const std::string name;
    const int32_t type;
    const int32_t character_apparition_type;
    const int32_t round;
    const bool hasQuest;
    Event(
      std::string name,
      int32_t type,
      int32_t character_apparition_type,
      int32_t round,
      bool hasQuest,
      Quest * quest,
      std::list<Character *> characters,
      std::list<MapLink *> links,
      Speech * speech,
      std::list<Effect *> effects
    ):
      name(name),
      type(type),
      character_apparition_type(character_apparition_type),
      round(round),
      hasQuest(hasQuest),
      quest(quest),
      characters(characters),
      links(links),
      speech(speech),
      effects(effects)
    {}
    Event(const Event * event):
      name(event->name),
      type(event->type),
      character_apparition_type(event->character_apparition_type),
      round(event->round),
      hasQuest(event->hasQuest),
      quest(event->quest),
      characters(event->characters),
      links(event->links),
      speech(event->speech),
      effects(event->effects)
    {}
    void activate(Adventure * adventure);
    bool operator == (const Event& e) const { return id == e.id; }
    bool operator != (const Event& e) const { return !operator==(e); }
  private:
    Quest * quest;
    std::list<Character *> characters;
    std::list<MapLink *> links;
    Speech * speech;
    std::list<Effect *> effects;
};

#endif // _EVENT_H_

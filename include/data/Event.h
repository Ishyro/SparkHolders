#ifndef _EVENT_H_
#define _EVENT_H_

#include <list>
#include <string>

#include "Values.h"

class Event {
  public:
    const std::string name;
    const int type;
    const int character_apparition_type;
    const int round;
    const bool hasQuest;
    Event(
      std::string name,
      int type,
      int character_apparition_type,
      int round,
      bool hasQuest,
      Quest * quest,
      std::list<Character *> characters,
      std::list<Map *> maps,
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
      maps(maps),
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
      maps(event->maps),
      links(event->links),
      speech(event->speech),
      effects(event->effects)
    {}
    void activate(Adventure * adventure);
  private:
    Quest * quest;
    std::list<Character *> characters;
    std::list<Map *> maps;
    std::list<MapLink *> links;
    Speech * speech;
    std::list<Effect *> effects;
};

#endif // _EVENT_H_

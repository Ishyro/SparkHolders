#ifndef _EVENT_H_
#define _EVENT_H_

#include <list>
#include <string>

#include "Values.h"

#include "data/Character.h"
#include "data/Map.h"
#include "data/Adventure.h"
#include "data/Speech.h"

#include "utils/FileOpener.h"

class Event {
  public:
    const std::string name;
    const int type;
    const int character_apparition_type;
    const int tick;
    const bool hasQuest;
    Event(
      std::string name,
      int type,
      int character_apparition_type,
      int tick,
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
      tick(tick),
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
      tick(event->tick),
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

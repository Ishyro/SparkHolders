#ifndef _EVENT_H_
#define _EVENT_H_

#include <list>
#include <string>

#include "Values.h"
#include "data/Character.h"
#include "data/Map.h"
#include "data/Adventure.h"
#include "data/Speech.h"

class Event { 
  public:
    const std::string name;
    const int type;
    const int character_apparition_type;
    const int tick;
    const bool hasQuest;
    const Quest * quest;
    Event(std::string name) { 
      // TODO XML_READER
    }
    void activate(Adventure * adventure);
  private:
    std::list<Character *> characters;
    std::list<Map *> maps;
    std::list<MapLink *> links;
    Speech * speech;
    std::list<Effect *> effects;
};

#endif // _EVENT_H_

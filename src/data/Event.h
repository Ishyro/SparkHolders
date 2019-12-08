#ifndef _EVENT_H_
#define _EVENT_H_

#include <list>
#include <string>

#include "src/Values.h"
#include "src/data/Character.h"
#include "src/data/Map.h"
#include "src/data/Speech.h"

class Event {
  public:
    const std::string name;
    const int type;
    const int tick;
    Event(std::string name) {
      // TODO XML_READER
    }
    void activate(World *  world);
  private:
    std::list<Character *> characters;
    std::list<Map *> maps;
    std::list<Speech *> speechs;
    std::list<Effect *> effects;
};

#endif // _EVENT_H_

#ifndef _ATTRIBUTES_H_
#define _ATTRIBUTES_H_

#include <list>
#include <string>

#include "src/data/Gear.h"

class Attributes { 
  public:
    const std::string name;
    const Gear * startingGear;
    const int hpIncr;
    const int manaIncr;
    const int armorIncr;
    const int soulBurnIncr;
    const int flowIncr;
    Attributes(std::string name) { 
      // TODO XML_READER
    }
};

#endif // _ATTRIBUTES_H_

#include <list>

#include "src/loader/MapLoader.h"
#include "src/data/Character.h"
#include "src/data/Projectile.h"
#include "src/data/Item.h"

class Class {
  public:
    const string name;
    const Gear startingGear;
    const int hpIncr;
    const int manaIncr;
    const int defenseIncr;
    const int soulBurnIncr;
    const int flowIncr;
    Class(string name);
};

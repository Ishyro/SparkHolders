#ifndef _ATTRIBUTES_H_
#define _ATTRIBUTES_H_

#include <list>
#include <string>

#include "data/Gear.h"

class Attributes {
  public:
    const std::string name;
    const int baseHp;
    const int baseMana;
    const int baseArmor;
    const int baseSoulBurn;
    const int baseFlow;
    const int baseVision;
    const int baseDarkVision;
    const Gear * startingGear;
    Attributes(
      std::string name,
      int baseHp,
      int baseMana,
      int baseArmor,
      int baseSoulBurn,
      int baseFlow,
      int baseVision,
      int baseDarkVision,
      Gear * startingGear
    ):
      baseHp(baseHp),
      baseMana(baseMana),
      baseArmor(baseArmor),
      baseSoulBurn(baseSoulBurn),
      baseFlow(baseFlow),
      baseVision(baseVision),
      baseDarkVision(baseDarkVision),
      startingGear(startingGear)
    {}
};

#endif // _ATTRIBUTES_H_

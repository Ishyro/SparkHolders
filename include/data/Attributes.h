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
    const Gear * startingGear;

    const int hpIncr;
    const int manaIncr;
    const int armorIncr;
    const int soulBurnIncr;
    const int flowIncr;
    Attributes(
      std::string name,
      int baseHp,
      int baseMana,
      int baseArmor,
      int baseSoulBurn,
      int baseFlow,
      Gear * startingGear,
      int hpIncr,
      int manaIncr,
      int armorIncr,
      int soulBurnIncr,
      int flowIncr
    ):
      baseHp(baseHp),
      baseMana(baseMana),
      baseArmor(baseArmor),
      baseSoulBurn(baseSoulBurn),
      baseFlow(baseFlow),
      startingGear(startingGear),
      hpIncr(hpIncr),
      manaIncr(manaIncr),
      armorIncr(armorIncr),
      soulBurnIncr(soulBurnIncr),
      flowIncr(flowIncr)
    {}
};

#endif // _ATTRIBUTES_H_

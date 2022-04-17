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
    const int baseVisionRange;
    const int baseVisionPower;
    const int baseDetectionRange;
    const Gear * startingGear;
    Attributes(
      std::string name,
      int baseHp,
      int baseMana,
      int baseArmor,
      int baseSoulBurn,
      int baseFlow,
      int baseVisionRange,
      int baseVisionPower,
      int baseDetectionRange,
      Gear * startingGear
    ):
      name(name),
      baseHp(baseHp),
      baseMana(baseMana),
      baseArmor(baseArmor),
      baseSoulBurn(baseSoulBurn),
      baseFlow(baseFlow),
      baseVisionRange(baseVisionRange),
      baseVisionPower(baseVisionPower),
      baseDetectionRange(baseDetectionRange),
      startingGear(startingGear)
    {}
    std::string to_string();
    static Attributes * from_string(std::string to_read);
};

#endif // _ATTRIBUTES_H_

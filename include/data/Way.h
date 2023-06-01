#ifndef _WAY_H_
#define _WAY_H_

#include <list>
#include <string>

#include "data/items/Item.h"

#include "Values.h"

class Way {
  public:
    const std::string name;
    const int type;
    const int baseHp;
    const int baseMana;
    const int baseArmor;
    const int baseArmorMult;
    const int baseDamageMult;
    const int baseSoulBurn;
    const int baseFlow;
    const int baseVisionRange;
    const int baseVisionPower;
    const int baseDetectionRange;
    const int hpIncr;
    const int manaIncr;
    const int armorMultIncr;
    const int damageMultIncr;
    const int soulBurnIncr;
    const int flowIncr;
    // race only
    const float size;
    const bool need_to_eat;
    const bool can_eat_food;
    const bool need_to_sleep;
    const float action_time_modifier;
    const float strike_time_modifier;
    const float skill_time_modifier;
    const float movement_time_modifier;
    Way(
      std::string name,
      int type,
      int baseHp,
      int baseMana,
      int baseArmor,
      int baseArmorMult,
      int baseDamageMult,
      int baseSoulBurn,
      int baseFlow,
      int baseVisionRange,
      int baseVisionPower,
      int baseDetectionRange,
      int hpIncr,
      int manaIncr,
      int armorMultIncr,
      int damageMultIncr,
      int soulBurnIncr,
      int flowIncr,
      float size,
      bool need_to_eat,
      bool can_eat_food,
      bool need_to_sleep,
      float action_time_modifier,
      float strike_time_modifier,
      float skill_time_modifier,
      float movement_time_modifier,
      std::list<Item *> loot,
      std::list<Effect *> effects,
      std::list<Skill *> skills
    ):
      name(name),
      type(type),
      baseHp(baseHp),
      baseMana(baseMana),
      baseArmor(baseArmor),
      baseArmorMult(baseArmorMult),
      baseDamageMult(baseDamageMult),
      baseSoulBurn(baseSoulBurn),
      baseFlow(baseFlow),
      baseVisionRange(baseVisionRange),
      baseVisionPower(baseVisionPower),
      baseDetectionRange(baseDetectionRange),
      hpIncr(hpIncr),
      manaIncr(manaIncr),
      armorMultIncr(armorMultIncr),
      damageMultIncr(damageMultIncr),
      soulBurnIncr(soulBurnIncr),
      flowIncr(flowIncr),
      size(size),
      need_to_eat(need_to_eat),
      can_eat_food(can_eat_food),
      need_to_sleep(need_to_sleep),
      action_time_modifier(action_time_modifier),
      strike_time_modifier(strike_time_modifier),
      skill_time_modifier(skill_time_modifier),
      movement_time_modifier(movement_time_modifier),
      loot(std::list<Item *>()),
      effects(effects),
      skills(skills)
    {
      for(Item * item : loot) {
        this->loot.push_back(Item::init(item, 1, 1));
      }
    }
    Way(
      std::string name,
      int type,
      int baseHp,
      int baseMana,
      int baseArmorMult,
      int baseDamageMult,
      int baseSoulBurn,
      int baseFlow,
      int baseVisionRange,
      int baseVisionPower,
      int baseDetectionRange,
      int hpIncr,
      int manaIncr,
      int armorMultIncr,
      int damageMultIncr,
      int soulBurnIncr,
      int flowIncr,
      std::list<Effect *> effects,
      std::list<Skill *> skills
    ):
      name(name),
      type(type),
      baseHp(baseHp),
      baseMana(baseMana),
      baseArmorMult(baseArmorMult),
      baseDamageMult(baseDamageMult),
      baseSoulBurn(baseSoulBurn),
      baseFlow(baseFlow),
      baseVisionRange(baseVisionRange),
      baseVisionPower(baseVisionPower),
      baseDetectionRange(baseDetectionRange),
      hpIncr(hpIncr),
      manaIncr(manaIncr),
      armorMultIncr(armorMultIncr),
      damageMultIncr(damageMultIncr),
      soulBurnIncr(soulBurnIncr),
      flowIncr(flowIncr),
      baseArmor(0),
      size(0.F),
      need_to_eat(true),
      can_eat_food(true),
      need_to_sleep(true),
      action_time_modifier(0.F),
      strike_time_modifier(0.F),
      skill_time_modifier(0.F),
      movement_time_modifier(0.F),
      loot(std::list<Item *>()),
      effects(effects),
      skills(skills)
    {}
    std::list<Item *> getLoot();
    std::list<Effect *> getEffects();
    std::list<Skill *> getSkills();
    bool operator == (const Way& w) const { return name == w.name; }
    bool operator != (const Way& w) const { return !operator==(w); }
  private:
    std::list<Effect *> effects;
    std::list<Skill *> skills;
    // race only
    std::list<Item *> loot;
};

#endif // _WAY_H_

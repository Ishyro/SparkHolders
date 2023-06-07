#ifndef _RACE_H_
#define _RACE_H_

#include <list>
#include <string>

#include "data/items/Item.h"

#include "data/Way.h"

#include "Values.h"

class Race : public Way {
  public:
    const float size;
    const int baseArmor;
    const bool need_to_eat;
    const bool can_eat_food;
    const bool need_to_sleep;
    const float action_time_modifier;
    const float strike_time_modifier;
    const float skill_time_modifier;
    const float movement_time_modifier;
    Race(
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
      std::list<Skill *> skills,
      float size,
      int baseArmor,
      bool need_to_eat,
      bool can_eat_food,
      bool need_to_sleep,
      float action_time_modifier,
      float strike_time_modifier,
      float skill_time_modifier,
      float movement_time_modifier,
      std::list<Item *> loot
    ):
      Way(
        name,
        type,
        baseHp,
        baseMana,
        baseArmorMult,
        baseDamageMult,
        baseSoulBurn,
        baseFlow,
        baseVisionRange,
        baseVisionPower,
        baseDetectionRange,
        hpIncr,
        manaIncr,
        armorMultIncr,
        damageMultIncr,
        soulBurnIncr,
        flowIncr,
        effects,
        skills
      ),
      size(size),
      baseArmor(baseArmor),
      need_to_eat(need_to_eat),
      can_eat_food(can_eat_food),
      need_to_sleep(need_to_sleep),
      action_time_modifier(action_time_modifier),
      strike_time_modifier(strike_time_modifier),
      skill_time_modifier(skill_time_modifier),
      movement_time_modifier(movement_time_modifier),
      loot(std::list<Item *>())
    {
      for(Item * item : loot) {
        this->loot.push_back(Item::init(item, 1, 1));
      }
    }
    int getBaseHp();
    int getBaseMana();
    int getBaseArmor();
    int getBaseArmorMult();
    int getBaseDamageMult();
    int getBaseSoulBurn();
    int getBaseFlow();
    int getBaseVisionRange();
    int getBaseVisionPower();
    int getBaseDetectionRange();
    int getHpIncr();
    int getManaIncr();
    int getArmorMultIncr();
    int getDamageMultIncr();
    int getSoulBurnIncr();
    int getFlowIncr();
    float getSize();
    bool getNeedToEat();
    bool getCanEatFood();
    bool getNeedToSleep();
    float getActionTimeModifier();
    float getStrikeTimeModifier();
    float getSkillTimeModifier();
    float getMovementTimeModifier();
    std::list<Effect *> getEffects();
    std::list<Skill *> getSkills();
    std::list<Item *> getLoot();
    std::list<Race *> getModifiers();
    void addModifier(Race * modifier);
  private:
    Race * modifier;
    std::list<Item *> loot;
};

#endif // _RACE_H_

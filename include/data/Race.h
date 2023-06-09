#ifndef _RACE_H_
#define _RACE_H_

#include <list>
#include <string>

#include "data/items/Item.h"

#include "data/Way.h"

#include "Values.h"

class Race : public Way {
  public:
    const int race_type;
    const float size;
    const int baseArmor;
    const bool need_to_eat;
    const bool can_eat_food;
    const bool need_to_sleep;
    const bool has_soulspark;
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
      int race_type,
      float size,
      int baseArmor,
      bool need_to_eat,
      bool can_eat_food,
      bool need_to_sleep,
      bool has_soulspark,
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
      race_type(race_type),
      size(size),
      baseArmor(baseArmor),
      need_to_eat(need_to_eat),
      can_eat_food(can_eat_food),
      need_to_sleep(need_to_sleep),
      has_soulspark(has_soulspark),
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
    int getType(std::list<Race *> race_modifiers);
    int getBaseHp(std::list<Race *> race_modifiers);
    int getBaseMana(std::list<Race *> race_modifiers);
    int getBaseArmor(std::list<Race *> race_modifiers);
    int getBaseArmorMult(std::list<Race *> race_modifiers);
    int getBaseDamageMult(std::list<Race *> race_modifiers);
    int getBaseSoulBurn(std::list<Race *> race_modifiers);
    int getBaseFlow(std::list<Race *> race_modifiers);
    int getBaseVisionRange(std::list<Race *> race_modifiers);
    int getBaseVisionPower(std::list<Race *> race_modifiers);
    int getBaseDetectionRange(std::list<Race *> race_modifiers);
    int getHpIncr(std::list<Race *> race_modifiers);
    int getManaIncr(std::list<Race *> race_modifiers);
    int getArmorMultIncr(std::list<Race *> race_modifiers);
    int getDamageMultIncr(std::list<Race *> race_modifiers);
    int getSoulBurnIncr(std::list<Race *> race_modifiers);
    int getFlowIncr(std::list<Race *> race_modifiers);
    float getSize(std::list<Race *> race_modifiers);
    bool getNeedToEat(std::list<Race *> race_modifiers);
    bool getCanEatFood(std::list<Race *> race_modifiers);
    bool getNeedToSleep(std::list<Race *> race_modifiers);
    bool getHasSoulSpark(std::list<Race *> race_modifiers);
    float getActionTimeModifier(std::list<Race *> race_modifiers);
    float getStrikeTimeModifier(std::list<Race *> race_modifiers);
    float getSkillTimeModifier(std::list<Race *> race_modifiers);
    float getMovementTimeModifier(std::list<Race *> race_modifiers);
    std::list<Effect *> getEffects(std::list<Race *> race_modifiers);
    std::list<Skill *> getSkills(std::list<Race *> race_modifiers);
    std::list<Item *> getLoot(std::list<Race *> race_modifiers);
  private:
    std::list<Item *> loot;
};

#endif // _RACE_H_

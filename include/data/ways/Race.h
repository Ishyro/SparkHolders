#ifndef _RACE_H_
#define _RACE_H_

#include <list>
#include <string>

#include "data/items/Item.h"

#include "data/ways/Way.h"

#include "Values.h"

class Race : public Way {
  public:
    const int32_t race_type;
    const float size;
    const float height;
    const bool need_to_eat;
    const bool can_eat_food;
    const bool need_to_sleep;
    const bool need_to_think;
    const bool has_soulspark;
    const float action_time_modifier;
    const float strike_time_modifier;
    const float skill_time_modifier;
    const float movement_time_modifier;
    Race(
      std::string name,
      int32_t type,
      int32_t tier,
      int32_t baseHp,
      int32_t baseMana,
      int32_t baseShield,
      int32_t baseDamageMult,
      int32_t baseSoulBurn,
      int32_t baseFlow,
      int32_t baseVisionRange,
      int32_t baseVisionPower,
      int32_t baseDetectionRange,
      int32_t hpIncr,
      int32_t manaIncr,
      int32_t shieldIncr,
      int32_t damageMultIncr,
      int32_t soulBurnIncr,
      int32_t flowIncr,
      std::list<Effect *> effects,
      std::list<Skill *> skills,
      std::list<Stance *> stances,
      std::list<std::string> tags,
      int32_t race_type,
      float size,
      float height,
      bool need_to_eat,
      bool can_eat_food,
      bool need_to_sleep,
      bool need_to_think,
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
        tier,
        baseHp,
        baseMana,
        baseShield,
        baseDamageMult,
        baseSoulBurn,
        baseFlow,
        baseVisionRange,
        baseVisionPower,
        baseDetectionRange,
        hpIncr,
        manaIncr,
        shieldIncr,
        damageMultIncr,
        soulBurnIncr,
        flowIncr,
        effects,
        skills,
        stances,
        tags
      ),
      race_type(race_type),
      size(size),
      height(height),
      need_to_eat(need_to_eat),
      can_eat_food(can_eat_food),
      need_to_sleep(need_to_sleep),
      need_to_think(need_to_think),
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
    int32_t getType(std::list<Race *> race_modifiers);
    int32_t getBaseHp(std::list<Race *> race_modifiers);
    int32_t getBaseMana(std::list<Race *> race_modifiers);
    int32_t getBaseShield(std::list<Race *> race_modifiers);
    int32_t getBaseDamageMult(std::list<Race *> race_modifiers);
    int32_t getBaseSoulBurn(std::list<Race *> race_modifiers);
    int32_t getBaseFlow(std::list<Race *> race_modifiers);
    int32_t getBaseVisionRange(std::list<Race *> race_modifiers);
    int32_t getBaseVisionPower(std::list<Race *> race_modifiers);
    int32_t getBaseDetectionRange(std::list<Race *> race_modifiers);
    int32_t getHpIncr(std::list<Race *> race_modifiers);
    int32_t getManaIncr(std::list<Race *> race_modifiers);
    int32_t getShieldIncr(std::list<Race *> race_modifiers);
    int32_t getDamageMultIncr(std::list<Race *> race_modifiers);
    int32_t getSoulBurnIncr(std::list<Race *> race_modifiers);
    int32_t getFlowIncr(std::list<Race *> race_modifiers);
    float getSize(std::list<Race *> race_modifiers);
    float getHeight(std::list<Race *> race_modifiers);
    bool getNeedToEat(std::list<Race *> race_modifiers);
    bool getCanEatFood(std::list<Race *> race_modifiers);
    bool getNeedToSleep(std::list<Race *> race_modifiers);
    bool getNeedToThink(std::list<Race *> race_modifiers);
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

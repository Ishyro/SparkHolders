#ifndef _EFFECT_H_
#define _EFFECT_H_

#include <string>

#include "Values.h"

#include "data/Character.h"
#include "data/items/Gear.h"

#include "util/FileOpener.h"

namespace effect {
  static int64_t id_counter = 0;
}

class Effect {
  public:
    const int64_t id;
    const std::string name;
    const int32_t level;
    const std::string attributes;
    const int32_t type;
    const int32_t duration_type;
    const float power;
    const int32_t duration;
    Effect(
      std::string name,
      int64_t id,
      int32_t level,
      std::string attributes,
      int32_t type,
      int32_t duration_type,
      float power,
      int32_t duration,
      std::array<float, DAMAGE_TYPE_NUMBER> damages
    ):
      name(name),
      id(id),
      level(level),
      attributes(attributes),
      type(type),
      duration_type(duration_type),
      power(power),
      duration(duration)
    {
      tick_left=(duration_type != DURATION_INFINITE ? duration : -1);
      for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        this->damages[i] = damages[i];
      }
    }
    Effect(
      std::string name,
      int64_t id,
      int32_t level,
      std::string attributes,
      int32_t type,
      int32_t duration_type,
      float power,
      int32_t duration,
      int32_t tick_left,
      std::array<float, DAMAGE_TYPE_NUMBER> damages
    ):
      name(name),
      id(id),
      level(level),
      attributes(attributes),
      type(type),
      duration_type(duration_type),
      power(power),
      duration(duration),
      tick_left(tick_left)
    {
      for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        this->damages[i] = damages[i];
      }
    }
    Effect(
      Effect * base,
      float overcharge_power,
      float overcharge_duration
    ):
      name(base->name),
      id(++effect::id_counter),
      level(base->level),
      attributes(base->attributes),
      type(base->type),
      duration_type(base->duration_type),
      power(base->power * overcharge_power),
      duration(base->duration * overcharge_duration)
    {
      tick_left=duration;
      for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        this->damages[i] = base->damages[i] * overcharge_power;
      }
    }
    Effect(
      Effect * base,
      float overcharge_power,
      float overcharge_duration,
      int32_t scaling_type,
      std::array<float, DAMAGE_TYPE_NUMBER> damage_multipliers,
      Character * owner
    ):
      name(base->name),
      id(++effect::id_counter),
      level(base->level),
      attributes(base->attributes),
      type(base->type),
      duration_type(base->duration_type),
      power(base->power * overcharge_power),
      duration(base->duration * overcharge_duration)
    {
      tick_left=duration;
      for(int32_t i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        switch(scaling_type) {
          case SKILL_SCALE_NONE:
            this->damages[i] = base->damages[i] * overcharge_power;
            break;
          case SKILL_SCALE_MAIN_WEAPON:
            this->damages[i] = (base->damages[i] + owner->getGear()->getWeapon_1()->getDamageFromType(i) + owner->getGear()->getWeapon_1()->getDamageFromType(DAMAGE_PHYSICAL) * damage_multipliers[i]) * overcharge_power;
            break;
          case SKILL_SCALE_SUB_WEAPON:
            this->damages[i] = (base->damages[i] + owner->getGear()->getWeapon_2()->getDamageFromType(i) + owner->getGear()->getWeapon_2()->getDamageFromType(DAMAGE_PHYSICAL) * damage_multipliers[i]) * overcharge_power;
            break;
        }
      }
    }
    float getRawDamage();
    float getDamageFromType(int32_t damage_type);
    float getDamageReductionFromType(int32_t damage_type);
    int32_t getTickLeft();
    void activate(Character * target);
    void desactivate(Character * target);
    bool tick(Character * target); // if true, delete effect
    bool isSimilar(Effect * e);
    std::string to_string();
    static Effect * from_string(std::string msg);
    bool operator == (const Effect& e) const { return id == e.id; }
    bool operator != (const Effect& e) const { return !operator==(e); }
  private:
    int32_t tick_left;
    Character * owner;
    std::array<float, DAMAGE_TYPE_NUMBER> damages;
};

#endif // _EFFECT_H_

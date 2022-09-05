#ifndef _EFFECT_H_
#define _EFFECT_H_

#include <string>

#include "Values.h"

#include "data/Character.h"

#include "utils/FileOpener.h"

namespace effect {
  static long id_cpt = 0;
}

class Effect {
  public:
    const long id = ++effect::id_cpt;
    const std::string name;
    const int level;
    const std::string attributes;
    const int type;
    const int duration_type;
    const int power;
    const int duration;
    Effect(
      std::string name,
      int level,
      std::string attributes,
      int type,
      int duration_type,
      int power,
      int duration,
      int damages[DAMAGE_TYPE_NUMBER],
      float damage_reductions[DAMAGE_TYPE_NUMBER]
    ):
      name(name),
      level(level),
      attributes(attributes),
      type(type),
      duration_type(duration_type),
      power(power),
      duration(duration)
    {
      tick_left=(duration_type != INFINITE ? duration : -1);
      for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        this->damages[i] = damages[i];
        this->damage_reductions[i] = damage_reductions[i];
      }
    }
    Effect(
      std::string name,
      int level,
      std::string attributes,
      int type,
      int duration_type,
      int power,
      int duration,
      int tick_left,
      int damages[DAMAGE_TYPE_NUMBER],
      float damage_reductions[DAMAGE_TYPE_NUMBER]
    ):
      name(name),
      level(level),
      attributes(attributes),
      type(type),
      duration_type(duration_type),
      power(power),
      duration(duration),
      tick_left(tick_left)
    {
      for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        this->damages[i] = damages[i];
        this->damage_reductions[i] = damage_reductions[i];
      }
    }
    Effect(
      Effect * base,
      int overcharge_power,
      int overcharge_duration
    ):
      name(base->name),
      level(base->level),
      attributes(base->attributes),
      type(base->type),
      duration_type(base->duration_type),
      power(base->power * overcharge_power),
      duration(base->duration * overcharge_duration)
    {
      tick_left=duration;
      for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        this->damages[i] = base->damages[i] * overcharge_power;
        this->damage_reductions[i] = damage_reductions[i] * overcharge_power;
      }
    }
    int getRawDamage();
    int getDamageFromType(int damage_type);
    float getDamageReductionFromType(int damage_type);
    int getTickLeft();
    void activate(Character * target);
    void desactivate(Character * target);
    bool tick(Character * target); // if true, delete effect
    std::string to_string();
    static Effect * from_string(std::string msg);
    bool operator == (const Effect& e) const { return id == e.id; }
    bool operator != (const Effect& e) const { return !operator==(e); }
  private:
    int tick_left;
    int damages[DAMAGE_TYPE_NUMBER];
    float damage_reductions[DAMAGE_TYPE_NUMBER];
};

#endif // _EFFECT_H_

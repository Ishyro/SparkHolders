#ifndef _EFFECT_H_
#define _EFFECT_H_

#include <string>

#include "Values.h"

#include "data/Character.h"

#include "utils/FileOpener.h"

class Effect {
  public:
    const std::string name;
    const int type;
    const int duration_type;
    const int power;
    const int duration;
    Effect(
      std::string name,
      int type,
      int duration_type,
      int power,
      int duration,
      int damages[DAMAGE_TYPE_NUMBER]
    ):
      name(name),
      type(type),
      duration_type(duration_type),
      power(power),
      duration(duration)
    {
      tick_left=duration;
      for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        this->damages[i] = damages[i];
      }
    }
    Effect(Effect * base, long overcharge):
      name(base->name),
      type(base->type),
      duration_type(base->duration_type),
      power(base->power * overcharge),
      duration(base->duration * overcharge)
    {
      for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        this->damages[i] = base->damages[i] * overcharge;
      }
    }
    int getRawDamage();
    int getDamageFromType(int damage_type);
    int getTickLeft();
    void activate(Character * target);
    void desactivate(Character * target);
    bool tick(Character * target); // if true, delete effect
    std::string to_string();
  private:
    int tick_left;
    int damages[DAMAGE_TYPE_NUMBER];
};

#endif // _EFFECT_H_

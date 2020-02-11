#ifndef _EFFECT_H_
#define _EFFECT_H_

#include <string>

#include "src/Values.h"
#include "src/data/Character.h"

class Effect {
  public:
    const std::string name;
    const int type;
    const int duration_type;
    const int power;
    const int duration;
    Effect(std::string name) {
      // TODO XML_READER
    }
    Effect(Effect * base, int overcharge):
      name(base->name),
      type(base->type),
      duration_type(base->duration_type),
      power(base->power * overcharge),
      duration(base->duration * overcharge)
    {
      for (int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        this->damages[i] = base->damages[i] * overcharge;
      }
    }
    int getRawDamage();
    int getDamageType(int damage_type);
    int getTickLeft();
    void activate(Character * target);
    void desactivate(Character * target);
    bool tick(Character * target); // if true, delete effect
  private:
    int tick_left;
    int damages[DAMAGE_TYPE_NUMBER];
};

#endif // _EFFECT_H_

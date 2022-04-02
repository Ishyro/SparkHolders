#ifndef _ACTION_H_
#define _ACTION_H_

#include <list>
#include <string>
#include <cmath>
#include <algorithm>

#include "Values.h"

class Action {
  public:
    const int type;
    const int orientation;
    const Projectile * projectile;
    const Skill * skill;
    const Character * target;
    const MapLink * link;
    const Item * item;
    const Weapon * weapon;
    Action(
      const int type,
      Character * user,
      const int orientation,
      const Projectile * projectile,
      const Skill * skill,
      const Character * target,
      const MapLink * link,
      const Item * item,
      const Weapon * weapon
    ):
      type(type),
      user(user),
      orientation(orientation),
      projectile(projectile),
      skill(skill),
      target(target),
      link(link),
      item(item),
      weapon(weapon)
    {}
    void execute(Adventure * adventure);
  private:
    Character * user;
};

#endif // _ACTION_H_

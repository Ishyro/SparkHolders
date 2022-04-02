#ifndef _ACTION_H_
#define _ACTION_H_

#include <list>
#include <string>
#include <cmath>
#include <algorithm>

#include "data/Adventure.h"
#include "data/Attributes.h"
#include "data/Character.h"
#include "data/Effect.h"
#include "data/Event.h"
#include "data/Item.h"
#include "data/Map.h"
#include "data/Projectile.h"
#include "data/Quest.h"
#include "data/skills/Skill.h"
#include "data/Speech.h"
#include "data/Tile.h"
#include "data/Way.h"
#include "data/Weapon.h"
#include "data/World.h"

#include "data/Database.h"

#include "Values.h"

class Action {
  public:
    const int type;
    const Projectile * projectile;
    const Skill * skill;
    const Character * target;
    const MapLink * link;
    const Item * item;
    const Weapon * weapon;
    const int orientation;
    Action(
      const int type,
      Character * user,
      const Projectile * projectile,
      const Skill * skill,
      const Character * target,
      const MapLink * link,
      const Item * item,
      const Weapon * weapon,
      const int orientation
    ):
      type(type),
      user(user),
      projectile(projectile),
      skill(skill),
      target(target),
      link(link),
      item(item),
      weapon(weapon),
      orientation(orientation)
    {}
    void execute(Adventure * adventure);
  private:
    Character * user;
};

#endif // _ACTION_H_

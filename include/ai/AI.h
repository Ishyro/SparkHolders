#ifndef _AI_H_
#define _AI_H_

#include <map>
#include <string>
#include <cmath>
#include <algorithm>

#include "Values.h"

#include "data/Action.h"
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

class AI {
  public:
    const std::string name;
    AI(
      std::string name
    ):
      name(name)
    {}
    Action * getAction(Adventure * adventure, Character * c);
};

#endif // _AI_H_

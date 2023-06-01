#include "data/Way.h"

#include "data/Effect.h"
#include "data/skills/Skill.h"

#include "util/String.h"

std::list<Item *> Way::getLoot() { return loot; }
std::list<Effect *> Way::getEffects() { return effects; }
std::list<Skill *> Way::getSkills() { return skills; }

#include "data/Attributes.h"

#include "data/Effect.h"
#include "data/items/Gear.h"
#include "data/skills/Skill.h"

#include "util/String.h"

void Attributes::init(Attributes * archetype) {
  this->archetype = archetype;
}

Attributes * Attributes::getArchetype() { return archetype; }
std::list<Effect *> Attributes::getEffects() { return effects; }
std::list<Skill *> Attributes::getSkills() { return skills; }

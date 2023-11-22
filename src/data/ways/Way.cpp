#include "data/ways/Way.h"

#include "data/Effect.h"
#include "data/skills/Skill.h"

#include "util/String.h"

std::list<Effect *> Way::getEffects() { return effects; }
std::list<Skill *> Way::getSkills() { return skills; }
std::list<std::string> Way::getTags() { return tags; }

bool Way::hasTag(std::string tag) {
  for(std::string current_tag : tags) {
    if(tag == current_tag) {
      return true;
    }
  }
  return false;
}

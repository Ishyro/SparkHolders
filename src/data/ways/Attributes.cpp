#include "data/ways/Attributes.h"

#include "data/Character.h"

#include "util/String.h"

#include <set>

bool Attributes::respectRequirements(Character * character) {
  std::set<std::string> tags = character->getTags();
  for(std::string tag : and_requirements) {
    if(tags.count(tag) < 1) {
      return false;
    }
  }
  for(std::string tag : not_requirements) {
    if(tags.count(tag) > 0) {
      return false;
    }
  }
  if(or_requirements.empty()) {
    return true;
  }
  else {
    for(std::string tag : or_requirements) {
      if(tags.count(tag) < 1) {
        return true;
      }
    }
  }
  return false;
}

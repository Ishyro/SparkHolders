#include "data/ways/Race.h"

int32_t Race::getType(std::list<Race *> race_modifiers) {
  int32_t result = race_type;
  for(Race * modifier : race_modifiers) {
    result = std::max(result, modifier->type);
  }
  return result;
}

int32_t Race::getBaseHp(std::list<Race *> race_modifiers) {
  int32_t result = baseHp;
  for(Race * modifier : race_modifiers) {
    result += modifier->baseHp;
  }
  return result;
}

int32_t Race::getBaseMana(std::list<Race *> race_modifiers) {
  int32_t result = baseMana;
  for(Race * modifier : race_modifiers) {
    result += modifier->baseMana;
  }
  return result;
}

int32_t Race::getBaseShield(std::list<Race *> race_modifiers) {
  int32_t result = baseShield;
  for(Race * modifier : race_modifiers) {
    result += modifier->baseShield;
  }
  return result;
}

int32_t Race::getBaseDamageMult(std::list<Race *> race_modifiers) {
  int32_t result = baseDamageMult;
  for(Race * modifier : race_modifiers) {
    result += modifier->baseDamageMult;
  }
  return result;
}

int32_t Race::getBaseSoulBurn(std::list<Race *> race_modifiers) {
  int32_t result = baseSoulBurn;
  for(Race * modifier : race_modifiers) {
    result += modifier->baseSoulBurn;
  }
  return result;
}

int32_t Race::getBaseFlow(std::list<Race *> race_modifiers) {
  int32_t result = baseFlow;
  for(Race * modifier : race_modifiers) {
    result += modifier->baseFlow;
  }
  return result;
}

int32_t Race::getBaseVisionRange(std::list<Race *> race_modifiers) {
  int32_t result = baseVisionRange;
  for(Race * modifier : race_modifiers) {
    result += modifier->baseVisionRange;
  }
  return result;
}

int32_t Race::getBaseVisionPower(std::list<Race *> race_modifiers) {
  int32_t result = baseVisionPower;
  for(Race * modifier : race_modifiers) {
    result += modifier->baseVisionPower;
  }
  return result;
}

int32_t Race::getBaseDetectionRange(std::list<Race *> race_modifiers) {
  int32_t result = baseDetectionRange;
  for(Race * modifier : race_modifiers) {
    result += modifier->baseDetectionRange;
  }
  return result;
}

int32_t Race::getHpIncr(std::list<Race *> race_modifiers) {
  int32_t result = hpIncr;
  for(Race * modifier : race_modifiers) {
    result += modifier->hpIncr;
  }
  return result;
}

int32_t Race::getManaIncr(std::list<Race *> race_modifiers) {
  int32_t result = manaIncr;
  for(Race * modifier : race_modifiers) {
    result += modifier->manaIncr;
  }
  return result;
}

int32_t Race::getShieldIncr(std::list<Race *> race_modifiers) {
  int32_t result = shieldIncr;
  for(Race * modifier : race_modifiers) {
    result += modifier->shieldIncr;
  }
  return result;
}

int32_t Race::getDamageMultIncr(std::list<Race *> race_modifiers) {
  int32_t result = damageMultIncr;
  for(Race * modifier : race_modifiers) {
    result += modifier->damageMultIncr;
  }
  return result;
}

int32_t Race::getSoulBurnIncr(std::list<Race *> race_modifiers) {
  int32_t result = soulBurnIncr;
  for(Race * modifier : race_modifiers) {
    result += modifier->soulBurnIncr;
  }
  return result;
}

int32_t Race::getFlowIncr(std::list<Race *> race_modifiers) {
  int32_t result = flowIncr;
  for(Race * modifier : race_modifiers) {
    result += modifier->flowIncr;
  }
  return result;
}

MathUtil::Vector3 Race::getSize(std::list<Race *> race_modifiers) {
  MathUtil::Vector3 result = size;
  for(Race * modifier : race_modifiers) {
    result.x = std::max(result.x, modifier->size.x);
    result.y = std::max(result.y, modifier->size.y);
    result.z = std::max(result.z, modifier->size.z);
  }
  return result;
}

bool Race::getNeedToEat(std::list<Race *> race_modifiers) {
  bool result = need_to_eat;
  for(Race * modifier : race_modifiers) {
    result = result && modifier->need_to_eat;
  }
  return result;
}

bool Race::getCanEatFood(std::list<Race *> race_modifiers) {
  bool result = can_eat_food;
  for(Race * modifier : race_modifiers) {
    result = result && modifier->can_eat_food;
  }
  return result;
}

bool Race::getNeedToSleep(std::list<Race *> race_modifiers) {
  bool result = need_to_sleep;
  for(Race * modifier : race_modifiers) {
    result = result && modifier->need_to_sleep;
  }
  return result;
}

bool Race::getNeedToThink(std::list<Race *> race_modifiers) {
  bool result = need_to_think;
  for(Race * modifier : race_modifiers) {
    result = result && modifier->need_to_think;
  }
  return result;
}

bool Race::getHasSoulSpark(std::list<Race *> race_modifiers) {
  bool result = has_soulspark;
  for(Race * modifier : race_modifiers) {
    result = result || modifier->has_soulspark;
  }
  return result;
}

float Race::getActionTimeModifier(std::list<Race *> race_modifiers) {
  float result = action_time_modifier;
  for(Race * modifier : race_modifiers) {
    result *= modifier->action_time_modifier;
  }
  return result;
}

float Race::getStrikeTimeModifier(std::list<Race *> race_modifiers) {
  float result = strike_time_modifier;
  for(Race * modifier : race_modifiers) {
    result *= modifier->strike_time_modifier;
  }
  return result;
}

float Race::getSkillTimeModifier(std::list<Race *> race_modifiers) {
  float result = skill_time_modifier;
  for(Race * modifier : race_modifiers) {
    result *= modifier->skill_time_modifier;
  }
  return result;
}

float Race::getMovementTimeModifier(std::list<Race *> race_modifiers) {
  float result = movement_time_modifier;
  for(Race * modifier : race_modifiers) {
    result *= modifier->movement_time_modifier;
  }
  return result;
}

std::list<Effect *> Race::getEffects(std::list<Race *> race_modifiers) {
  std::list<Effect *> result = std::list<Effect *>(effects);
  for(Race * modifier : race_modifiers) {
    for(Effect * effect : modifier->effects) {
      result.push_back(effect);
    }
  }
  return result;
}

std::list<Skill *> Race::getSkills(std::list<Race *> race_modifiers) {
  std::list<Skill *> result = std::list<Skill *>(skills);
  for(Race * modifier : race_modifiers) {
    for(Skill * skill : modifier->skills) {
      result.push_back(skill);
    }
  }
  return result;
}

std::list<Item *> Race::getLoot(std::list<Race *> race_modifiers) {
  std::list<Item *> result = std::list<Item *>(loot);
  for(Race * modifier : race_modifiers) {
    for(Item * item : modifier->loot) {
      result.push_back(item);
    }
  }
  return result;
}

#include "data/ways/Race.h"

int Race::getType(std::list<Race *> race_modifiers) {
  int result = race_type;
  for(Race * modifier : race_modifiers) {
    result = std::max(result, modifier->type);
  }
  return result;
}

int Race::getBaseHp(std::list<Race *> race_modifiers) {
  int result = baseHp;
  for(Race * modifier : race_modifiers) {
    result += modifier->baseHp;
  }
  return result;
}

int Race::getBaseMana(std::list<Race *> race_modifiers) {
  int result = baseMana;
  for(Race * modifier : race_modifiers) {
    result += modifier->baseMana;
  }
  return result;
}

int Race::getBaseShield(std::list<Race *> race_modifiers) {
  int result = baseShield;
  for(Race * modifier : race_modifiers) {
    result += modifier->baseShield;
  }
  return result;
}

int Race::getBaseDamageMult(std::list<Race *> race_modifiers) {
  int result = baseDamageMult;
  for(Race * modifier : race_modifiers) {
    result += modifier->baseDamageMult;
  }
  return result;
}

int Race::getBaseSoulBurn(std::list<Race *> race_modifiers) {
  int result = baseSoulBurn;
  for(Race * modifier : race_modifiers) {
    result += modifier->baseSoulBurn;
  }
  return result;
}

int Race::getBaseFlow(std::list<Race *> race_modifiers) {
  int result = baseFlow;
  for(Race * modifier : race_modifiers) {
    result += modifier->baseFlow;
  }
  return result;
}

int Race::getBaseVisionRange(std::list<Race *> race_modifiers) {
  int result = baseVisionRange;
  for(Race * modifier : race_modifiers) {
    result += modifier->baseVisionRange;
  }
  return result;
}

int Race::getBaseVisionPower(std::list<Race *> race_modifiers) {
  int result = baseVisionPower;
  for(Race * modifier : race_modifiers) {
    result += modifier->baseVisionPower;
  }
  return result;
}

int Race::getBaseDetectionRange(std::list<Race *> race_modifiers) {
  int result = baseDetectionRange;
  for(Race * modifier : race_modifiers) {
    result += modifier->baseDetectionRange;
  }
  return result;
}

int Race::getHpIncr(std::list<Race *> race_modifiers) {
  int result = hpIncr;
  for(Race * modifier : race_modifiers) {
    result += modifier->hpIncr;
  }
  return result;
}

int Race::getManaIncr(std::list<Race *> race_modifiers) {
  int result = manaIncr;
  for(Race * modifier : race_modifiers) {
    result += modifier->manaIncr;
  }
  return result;
}

int Race::getShieldIncr(std::list<Race *> race_modifiers) {
  int result = shieldIncr;
  for(Race * modifier : race_modifiers) {
    result += modifier->shieldIncr;
  }
  return result;
}

int Race::getDamageMultIncr(std::list<Race *> race_modifiers) {
  int result = damageMultIncr;
  for(Race * modifier : race_modifiers) {
    result += modifier->damageMultIncr;
  }
  return result;
}

int Race::getSoulBurnIncr(std::list<Race *> race_modifiers) {
  int result = soulBurnIncr;
  for(Race * modifier : race_modifiers) {
    result += modifier->soulBurnIncr;
  }
  return result;
}

int Race::getFlowIncr(std::list<Race *> race_modifiers) {
  int result = flowIncr;
  for(Race * modifier : race_modifiers) {
    result += modifier->flowIncr;
  }
  return result;
}

float Race::getSize(std::list<Race *> race_modifiers) {
  float result = size;
  for(Race * modifier : race_modifiers) {
    result = std::max(result, modifier->size);
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

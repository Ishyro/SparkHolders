#include "data/Effect.h"

void Effect::activate(Character * target) {
  if(duration != 0) {
    target->addEffect(this);
  }
  else {
    switch (type) {
      // no duration means INSTANT for these effects
      case HP:
        target->hpHeal(power);
        break;
      case MANA:
        target->manaHeal(power);
        break;
      case HP_MAX:
        target->incrMaxHp();
        break;
      case MANA_MAX:
        target->incrMaxMana();
        break;
      case ARMOR:
        target->incrArmor();
        break;
      case SOULBURNTRESHOLD:
        target->incrSoulBurnTreshold();
        break;
      case FLOW:
        target->incrFlow();
        break;
      case VISION_RANGE:
        target->incrVisionRange();
        break;
      case VISION_POWER:
        target->incrVisionPower();
        break;
      case DETECTION_RANGE:
        target->incrDetectionRange();
      case DAMAGE:
        for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
          target->receiveAttack(damages[i], i, NO_ORIENTATION);
        }
        break;
      // no duration means INFINITE for next effects (can be cancelled)
      default:
        target->addEffect(this);
    }
  }
}

bool Effect::tick(Character * target) {
  switch (type) {
    case HP:
      target->hpHeal(power);
      break;
    case MANA:
      target->manaHeal(power);
      break;
    case DAMAGE:
      for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        target->receiveAttack(damages[i], i, NO_ORIENTATION);
      }
      break;
    default:
      ;
  }
  return duration_type == TEMPORARY && --tick_left == 0;
}

void Effect::desactivate(Character * target) {
  if(duration_type == INFINITE || duration_type == TEMPORARY) {
    target->removeEffect(this);
  }
}

int Effect::getTickLeft() { return tick_left; }

int Effect::getRawDamage() {
  int power = 0;
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    power += damages[i];
  }
  return power;
}

int Effect::getDamageFromType(int damage_type) { return damages[damage_type]; }

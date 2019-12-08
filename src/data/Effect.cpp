#include "src/data/Effect.h"

bool Effect::decrTick() {
  if (duration_type == TEMPORARY)
    return --tick_left == 0;
  else return false;
}

void Effect::activate(Character * target) {
  if (duration != 0) {
    target->addEffect(this);
  }
  else {
    switch (type) {
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
      case VISION:
        target->setVision(target->getVision() + power);
        break;
    }
  }
}

void Effect::desactivate(Character * target) {
  if (duration_type == INFINITE || duration_type == TEMPORARY) {
    target->removeEffect(this);
  }
}

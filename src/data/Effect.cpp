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
      case VISION:
        target->setVision(target->getVision() + power);
        break;
      case DARK_VISION:
        target->setDarkVision(target->getDarkVision() + power);
        break;
      case DAMAGE:
        target->receiveAttack(power, damage_type);
        break;
      // no duration means INFINITE for next effects (can be cancelled)
      default:
        target->addEffect(this);
    }
  }
}

void Effect::desactivate(Character * target) {
  if (duration_type == INFINITE || duration_type == TEMPORARY) {
    target->removeEffect(this);
  }
}

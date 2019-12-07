#include "src/data/Effect.h"

Effect::Effect(std::string name) {
  // TODO
  // XML READER
}

bool Effect::decrTick() {
  return --tick_left == 0;
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
      case DEFENSE:
        target->incrDefense();
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

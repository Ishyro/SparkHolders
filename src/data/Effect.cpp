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

std::string Effect::to_string() {
  std::string msg = name + ".";
  msg += std::to_string(type) + ".";
  msg += std::to_string(duration_type) + ".";
  msg += std::to_string(power) + ".";
  msg += std::to_string(duration) + ".";
  msg += std::to_string(tick_left) + ".";
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    msg += std::to_string(damages[i]) + ".";
  }
  return msg;
}

Effect * from_string(std::string msg) {
  std::string name = msg.substr(0, msg.find('.'));
  msg = msg.substr(msg.find('.') + 1, msg.length());
  int type = stoi(msg.substr(0, msg.find('.')));
  msg = msg.substr(msg.find('.') + 1, msg.length());
  int duration_type = stoi(msg.substr(0, msg.find('.')));
  msg = msg.substr(msg.find('.') + 1, msg.length());
  int power = stoi(msg.substr(0, msg.find('.')));
  msg = msg.substr(msg.find('.') + 1, msg.length());
  int duration = stoi(msg.substr(0, msg.find('.')));
  msg = msg.substr(msg.find('.') + 1, msg.length());
  int tick_left = stoi(msg.substr(0, msg.find('.')));
  msg = msg.substr(msg.find('.') + 1, msg.length());
  int damages[DAMAGE_TYPE_NUMBER];
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    damages[i] = stoi(msg.substr(0, msg.find('.')));
    msg = msg.substr(msg.find('.') + 1, msg.length());
  }
  return new Effect(name, type, duration_type, power, duration, tick_left, damages);
 }

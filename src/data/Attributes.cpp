#include "data/Attributes.h"

std::string Attributes::to_string() {
  std::string msg = name + ";";
  msg += std::to_string(baseHp) + ";";
  msg += std::to_string(baseMana) + ";";
  msg += std::to_string(baseArmor) + ";";
  msg += std::to_string(baseSoulBurn) + ";";
  msg += std::to_string(baseFlow) + ";";
  msg += std::to_string(baseVisionRange) + ";";
  msg += std::to_string(baseVisionPower) + ";";
  msg += std::to_string(baseDetectionRange) + ";";
  return msg;
}

Attributes * Attributes::from_string(std::string toread) {
  std::string msg = toread;
  std::string name = msg.substr(0, msg.find(';'));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  int baseHp = stoi(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  int baseMana = stoi(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  int baseArmor = stoi(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  int baseSoulBurn = stoi(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  int baseFlow = stoi(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  int baseVisionRange = stoi(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  int baseVisionPower = stoi(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  int baseDetectionRange = stoi(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  return new Attributes(name, baseHp, baseMana, baseArmor, baseSoulBurn, baseFlow, baseVisionRange, baseVisionPower, baseDetectionRange, new Gear());
}

#include "data/Way.h"

std::list<Effect *> Way::getEffects() { return effects; }
std::list<Skill *> Way::getSkills() { return skills; }

std::string Way::to_string() {
  std::string msg = name + ";";
  msg += std::to_string(type) + ";";
  msg += std::to_string(hpIncr) + ";";
  msg += std::to_string(manaIncr) + ";";
  msg += std::to_string(armorIncr) + ";";
  msg += std::to_string(soulBurnIncr) + ";";
  msg += std::to_string(flowIncr) + ";";
  return msg;
}

Way * Way::from_string(std::string to_read) {
  std::string msg = to_read;
  std::string name = msg.substr(0, msg.find(';'));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  int type = stoi(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  int hpIncr = stoi(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  int manaIncr = stoi(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  int armorIncr = stoi(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  int soulBurnIncr = stoi(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  int flowIncr = stoi(msg.substr(0, msg.find(';')));
  msg = msg.substr(msg.find(';') + 1, msg.length());
  return new Way(name, type, hpIncr, manaIncr, armorIncr, soulBurnIncr, flowIncr, std::list<Effect *>(), std::list<Skill *>());
}

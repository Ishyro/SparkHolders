#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <vector>
#include <map>
#include <list>
#include <string>

#include "Values.h"

class Socket;

namespace Client {
  Adventure * receiveAdventure(std::string msg, int64_t & tickrate, bool & master);
  StateDisplay * receiveState(std::string msg, Adventure * adventure);
  std::string writeAction(int32_t type, void * arg1 = nullptr, void * arg2 = nullptr, int32_t mana_cost = 1);
  std::string writeBaseAction(int32_t type);
  std::string writeGearAction(int32_t type, ItemSlot * slot1, ItemSlot * slot2);
  std::string writeOrientedAction(int32_t type, MathUtil::Vector3 orientation);
  std::string writeTargetedAction(int32_t type, MathUtil::Target * target);
  std::string writeSkillAction(int32_t type, MathUtil::Target * target, Skill * skill, int32_t mana_cost);
  void sendChoices(Socket s, Adventure * adventure, std::string name, std::string attributes, std::string race, std::string origin, std::string culture, std::string religion, std::string profession);
}

#endif // _CLIENT_H_

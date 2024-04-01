#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <vector>
#include <map>
#include <list>
#include <string>

#include "Values.h"

class Socket;

namespace Client {
  Adventure * receiveAdventure(std::string msg, bool & master);
  StateDisplay * receiveState(std::string msg, Adventure * adventure);
  std::string writeActions(int64_t id, std::vector<int32_t> types, std::vector<void *> args1, std::vector<void *> args2, std::vector<int32_t> overcharge_powers, std::vector<int32_t> overcharge_durations, std::vector<int32_t> overcharge_ranges);
  std::string writeAction(int32_t type, void * arg1 = nullptr, void * arg2 = nullptr, int32_t overcharge_power = 1, int32_t overcharge_duration = 1, int32_t overcharge_range = 1);
  std::string writeBaseAction(int32_t type);
  std::string writeGearAction(int32_t type, ItemSlot * slot1, ItemSlot * slot2);
  std::string writeTargetedAction(int32_t type, Target * target);
  std::string writeSkillAction(int32_t type, Target * target, Skill * skill, int32_t overcharge_power, int32_t overcharge_duration, int32_t overcharge_range);
  void sendChoices(Socket s, Adventure * adventure, std::string name, std::string attributes, std::string race, std::string origin, std::string culture, std::string religion, std::string profession);
}

#endif // _CLIENT_H_

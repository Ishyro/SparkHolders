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
  std::string writeActions(long id, std::vector<int> types, std::vector<void *> args1, std::vector<void *> args2, std::vector<int> overcharge_powers, std::vector<int> overcharge_durations, std::vector<int> overcharge_ranges);
  std::string writeAction(int type, void * arg1 = nullptr, void * arg2 = nullptr, int overcharge_power = 1, int overcharge_duration = 1, int overcharge_range = 1);
  std::string writeBaseAction(int type);
  std::string writeGearAction(int type, long item_id);
  std::string writeTargetedAction(int type, Target * target);
  std::string writeSkillAction(int type, Target * target, Skill * skill, int overcharge_power, int overcharge_duration, int overcharge_range);
  void sendChoices(Socket s, Adventure * adventure, std::string name, std::string attributes, std::string race, std::string origin, std::string culture, std::string religion, std::string profession);
}

#endif // _CLIENT_H_

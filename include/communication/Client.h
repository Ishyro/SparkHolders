#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <vector>
#include <map>
#include <list>
#include <string>

#include "Values.h"

class Socket;

namespace Client {
  Adventure * receiveAdventure(Socket s);
  StateDisplay * receiveState(Socket s, Adventure * adventure, Character ** player, bool * need_action);
  void sendBaseAction(Socket s, int type);
  void sendGearAction(Socket s, int type, long item_id);
  void sendTargetedAction(Socket s, int type, Target * target);
  void sendSkillAction(Socket s, int type, Target * target, Skill * skill, int overcharge_power, int overcharge_duration, int overcharge_range);
  Character * sendChoices(Socket s, Adventure * adventure, std::string name, std::string attributes, std::string race, std::string origin, std::string culture, std::string religion, std::string profession);
}

#endif // _CLIENT_H_

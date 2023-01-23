#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <vector>
#include <map>
#include <list>
#include <string>

#include "Values.h"

class Socket;

namespace Client {
  void receiveStartingPossibilites(Socket s, std::vector<Attributes *> * attributes, std::vector<Way *> * ways);
  std::list<std::string> receiveTranslationPaths(Socket s);
  MapDisplay * receiveMap(Socket s, Character ** player, long * id);
  void receiveWaysIncompabilities(Socket s, std::list<std::pair<const std::string, const std::string>> * waysIncompatibilities);
  void sendAction(Socket s, int type, float orientation, Skill * skill, int target_id, int target_x, int target_y, std::string object, int overcharge_power, int overcharge_duration, int overcharge_range);
  Character * sendChoices(Socket s, std::string name, std::string attributes, std::string race, std::string origin, std::string culture, std::string religion, std::string profession);
}

#endif // _CLIENT_H_

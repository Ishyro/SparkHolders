#include <vector>
#include <map>
#include <list>
#include <string>

#include "Values.h"

class Socket;

namespace Client {
  void receiveStartingPossibilites(Socket s, std::vector<Attributes *> * attributes, std::vector<Way *> * ways);
  std::list<std::string> receiveTraductionPaths(Socket s);
  MapDisplay * receiveMap(Socket s);
  void receiveWaysIncompabilities(Socket s, std::list<std::pair<const std::string, const std::string>> * waysIncompatibilities);
  void sendAction(Socket s, int type, int orientation, Skill * skill, int target_id, int target_x, int target_y, std::string object, int overcharge);
  Character * sendChoices(Socket s, std::string name, std::string attibutes, std::string race, std::string origin, std::string culture, std::string religion, std::string profession);
}

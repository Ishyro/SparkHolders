#include <vector>

#include "Values.h"

class Socket;

namespace Client {
  MapDisplay * receiveMap(Socket s, Adventure * adventure);
  void receiveStartingPossibilites(Socket s, std::vector<Attributes *> * attributes, std::vector<Way *> * ways, Adventure * adventure);
  void sendAction(Socket s, int type, int orientation, ProjectileDisplay * projectile, Skill * skill, CharacterDisplay * target, Item * item, Weapon * weapon, Adventure * adventure);
  void sendChoices(Socket s, std::string name, std::string attibutes, std::string race, std::string origin, std::string culture, std::string religion, std::string profession, Adventure * adventure);
}

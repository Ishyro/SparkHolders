#include <list>

#include "Values.h"

class Socket;

namespace Client {
  MapDisplay * receiveMap(Socket s, Adventure * adventure);
  void receiveStartingPossibilites(Socket s, std::list<Attributes *> * attributes, std::list<Way *> * ways, Adventure * adventure);
  void sendAction(Socket s, Action * action, Adventure * adventure);
  void sendChoices(Socket s, std::string name, std::string attibutes, std::string race, std::string origin, std::string culture, std::string religion, std::string profession, Adventure * adventure);
}

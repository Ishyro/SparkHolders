#include "Values.h"

#include <map>

class Socket;

namespace Server {
  std::list<Action *> receiveActions(std::string msg, std::map<const long, Character *> characters, Adventure * adventure);
  Action * readAction(std::string msg, Character * user, Adventure * adventure);
  Character * receiveChoices(std::string msg, Adventure * adventure);
  void sendState(Socket s, std::map<const long, Character *> characters, bool need_to_send, Adventure * adventure);
  void sendAdventure(Socket s, Adventure * adventure, bool master);
  void sendCharacter(Socket s, Character * character, Adventure * adventure);
  void sendAction(Socket s);
}

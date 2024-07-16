#include "Values.h"

#include <map>

class Socket;

namespace Server {
  Action * receiveAction(std::string msg, Character * character, Adventure * adventure);
  Action * readAction(std::string msg, Character * user, Adventure * adventure);
  Character * receiveChoices(std::string msg, Adventure * adventure);
  void sendState(Socket s, Character * character, Adventure * adventure);
  void sendAdventure(Socket s, Adventure * adventure, bool master);
  void sendCharacter(Socket s, Character * character, Adventure * adventure);
  void sendAction(Socket s);
}

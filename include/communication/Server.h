#include "Values.h"

class Socket;

namespace Server {
  Action * receiveAction(Socket s, Character * user, Adventure * adventure);
  Character * receiveChoices(Socket s, Adventure * adventure);
  void sendState(Socket s, Character * player, Adventure * adventure);
  void sendAdventure(Socket s, Adventure * adventure);
}

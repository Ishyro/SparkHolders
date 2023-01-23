#include "Values.h"

class Socket;

namespace Server {
  Action * receiveAction(Socket s, Character * user, Adventure * adventure);
  Character * receiveChoices(Socket s, Adventure * adventure);
  void sendMap(Socket s, Map * map, Character * player, Adventure * adventure);
  void sendStartingPossibilites(Socket s, Adventure * adventure);
  void sendWaysIncompabilities(Socket s, Adventure * adventure);
  void sendTranslationPaths(Socket s, Adventure * adventure);
}

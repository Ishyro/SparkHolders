#include "Values.h"

class Socket;

namespace Server {
  Action * receiveAction(Socket s, Adventure * adventure);
  Character * receiveChoices(Socket s, Adventure * adventure);
  void sendMap(Socket s, Map * map);
  void sendStartingPossibilites(Socket s, Adventure * adventure);
}

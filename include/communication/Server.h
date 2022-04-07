#include "Values.h"

class Socket;

namespace Server {
  Action * receive(Socket s, Adventure * adventure);
  void send(Socket s, Map * map, Adventure * adventure);
}

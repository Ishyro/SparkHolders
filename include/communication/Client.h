#include "Values.h"

class Socket;

namespace Client {
  Map * receive(Socket s, Adventure * adventure);
  void send(Socket s, Action * action, Adventure * adventure);
}

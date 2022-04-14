#include "Values.h"

class Socket;

namespace Client {
  MapDisplay * receive(Socket s, Adventure * adventure);
  void send(Socket s, Action * action, Adventure * adventure);
}

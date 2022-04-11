#include <string>

#include "communication/Socket.h"

void communicate(Socket s) {
  for(std::string msg; msg != "CLOSED"; msg = s.read()) {
    if (msg != "") {
      std::cout << msg << std::endl;
    }
  }
}

int main(int argc, char ** argv) {
  std::string one  = "one";
  std::string two = "two";
  std::string three = "three";
  std::string foor = "foor";
  std::string five = "five";
  std::string six = "six";

  Socket s = Socket();
  s.connect("127.0.0.1", 45678);
  communicate(s);
  s.close();
}

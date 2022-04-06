#include <string>

#include "communication/Socket.h"

int main(int argc, char ** argv) {
  std::string one  = "one";
  std::string two = "two";
  std::string three = "three";
  std::string foor = "foor";
  std::string five = "five";
  std::string six = "six";

  Socket s = Socket();
  s.connect("127.0.0.1", 45678);
  s.write(one);
  s.write(two);
  s.write(three);
  s.write(foor);
  s.write(five);
  s.write(six);
  s.close();
}

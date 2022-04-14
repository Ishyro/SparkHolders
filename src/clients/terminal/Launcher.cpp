#include <string>

#include "communication/Client.h"
#include "communication/Socket.h"

#include "data/Map.h"

void displayMap(MapDisplay * display) {
  // clear terminal
  std::cout << "\033[2J\033[1;1H";
  std::cout << display->name << std::endl << std::endl;
  for(int x = 0; x < display->sizeX; x++) {
    for(int y = 0; y < display->sizeY; y++) {
      std::cout << display->tiles[x][y].at(0);
    }
    std::cout << std::endl;
  }
}

void communicate(Socket s) {
  while(true) {
    MapDisplay * display = Client::receive(s, nullptr);
    if(display != nullptr) {
      displayMap(display);
      delete display;
    }
  }
}

int main(int argc, char ** argv) {
  Socket s = Socket();
  s.connect("127.0.0.1", 45678);
  communicate(s);
  s.close();
}

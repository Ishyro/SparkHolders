#include <string>

#include "communication/Client.h"
#include "communication/Socket.h"

#include "clients/Link.h"

#include "data/Map.h"

void displayMap(MapDisplay * display) {
  // clear terminal
  std::cout << "\033[2J\033[1;1H";
  std::cout << display->name << std::endl << std::endl;
  for(int y = display->sizeX - 1; y >= 0 ; y--) {
    for(int x = 0; x < display->sizeX; x++) {
      if(display->tiles[x][y] != "") {
        std::cout << display->tiles[x][y].at(0);
      } else {
        std::cout << "m";
      }
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
  for(int y = display->sizeX - 1; y >= 0 ; y--) {
    for(int x = 0; x < display->sizeX; x++) {
      std::cout << display->lights[x][y];
    }
    std::cout << std::endl;
  }
}

void communicate(Link * link) {
  while(true) {
    MapDisplay * display = link->receiveMap();
    if(display != nullptr) {
      displayMap(display);
      for(CharacterDisplay * character : display->characters) {
        delete character;
      }
      for(ProjectileDisplay * projectile : display->projectiles) {
        delete projectile;
      }
      delete display;
    }
  }
}

int main(int argc, char ** argv) {
  Socket s = Socket();
  s.connect("127.0.0.1", 45678);
  Link * link = new Link(s, nullptr);
  link->sendChoices("Betatesteur", "Vanguard", "human", "countryside", "tagran", "atheist", "soldier");
  communicate(link);
  s.close();
}

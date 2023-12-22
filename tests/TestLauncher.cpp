#ifdef _WIN32_WINNT
  #include <winsock2.h>
  #include <windows.h>
  #include <thread>
#endif

#include <thread>
#include <vector>
#include <unistd.h>
#include <string>

#include "data/actions/Action.h"
#include "data/Adventure.h"
#include "data/Character.h"
#include "data/World.h"
#include "data/Region.h"
#include "data/BlocksChunk.h"
#include "data/Map.h"

#include "data/Settings.h"

#include "util/FileOpener.h"

#include "communication/Socket.h"
#include "communication/ServerSocket.h"
#include "communication/Server.h"
#include "communication/SpeechManager.h"

#include "server/Launcher.h"
#include "server/Link.h"

#include <random>

#include "Values.h"

#include <iostream>
#include <chrono>
#include <ctime>

int main(int argc, char ** argv) {

  if (argc < 3) {
    std::cerr << "Expected: ./Launcher <adventureFile.commands> <ticks>" << std::endl;
    return EXIT_FAILURE;
  }
  setlocale(LC_ALL, "");
  std::string adventureFile = argv[1];
  long ticks = std::stol(argv[2]);
  srand(Settings::getSeed());

  auto start = std::chrono::system_clock::now();
  Adventure * adventure = FileOpener::AdventureOpener(adventureFile, true);
  adventure->applyDayLight();
  for(int tick = 0; tick < ticks; tick++) {
    /*
    adventure->applyIteration();
    SpeechManager::clear();
    adventure->getNPCsActions();
    adventure->executeActions();
    adventure->actAllProjectiles();
    adventure->incrTick();
    */
  }
  Character * dasheep = adventure->getWorld()->getCharacters().front();
  for(float y = 0.F; y < 10.F; y += 0.001) {
    for(float x = 0.F; x < 10.F; x += 0.001) {
      float cost = MapUtil::round(dasheep->getRegion()->getMoveCost(dasheep, dasheep->getCoord(), MapUtil::makeVector3(x, y, 0)));
      float expected = MapUtil::round(MapUtil::distance(dasheep->getCoord(), MapUtil::makeVector3(x, y, 0)) * 10.F / dasheep->getMovementTimeModifier());
      if(cost != expected) {
        std::cout << "coord: " << x << " " << y << " cost: " << cost << " expected: " << expected << std::endl;
      }
    }
  }
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  std::cout << "Duration: " << elapsed_seconds.count() << "s" << std::endl;
  delete adventure;
}

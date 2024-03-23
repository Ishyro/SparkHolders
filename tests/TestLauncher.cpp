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
  for(Character * dasheep : adventure->getWorld()->getCharacters()) {
    for(auto pair : dasheep->getRegion()->getBlocks()) {
      if(pair.second != dasheep->getRegion()->getBlock(pair.first)) {
        std::cout << "coord: " << pair.first.x << " " << pair.first.y << " " << pair.first.z << std::endl;
        std::cout << "block: " << pair.second->name << std::endl;
      }
    }
  }
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  std::cout << "Duration: " << elapsed_seconds.count() << "s" << std::endl;
  delete adventure;
}

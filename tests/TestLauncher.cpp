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

int32_t main(int32_t argc, char ** argv) {

  if (argc < 3) {
    std::cerr << "Expected: ./Launcher <adventureFile.commands> <ticks>" << std::endl;
    return EXIT_FAILURE;
  }
  setlocale(LC_ALL, "");
  std::string adventureFile = argv[1];
  int64_t ticks = std::stol(argv[2]);
  srand(Settings::getSeed());

  auto start = std::chrono::system_clock::now();
  Adventure * adventure = FileOpener::AdventureOpener(adventureFile, true);
  Character * dasheep = adventure->getWorld()->getCharacters().front();
  dasheep->getRegion()->getBlocks();
  MathUtil::Coords coords = dasheep->getWorldCoords();
  Time time = adventure->getTime();
  std::cout << time.to_string_day() << std::endl;
  std::cout << "latitude: " << coords.latitude.x << "° " << coords.latitude.y << "' " << coords.latitude.z << "'' longitude: " << coords.longitude.x << "° " << coords.longitude.y << "' " << coords.longitude.z << "''" << std::endl;
  for(int32_t tick = 0; tick < ticks; tick++) {
    Time time = adventure->getTime();
    std::cout << time.to_string_clock() << std::endl;
    std::cout << MathUtil::getLight(coords, time) << std::endl;
    //
    adventure->applyIteration();
    SpeechManager::clear();
    adventure->getNPCsActions();
    adventure->executeActions();
    adventure->actAllProjectiles();
    adventure->incrTick();
  }
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  std::cout << "Duration: " << elapsed_seconds.count() << "s" << std::endl;
  delete adventure;
}

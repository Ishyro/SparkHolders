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
  MapUtil::Vector3 base_coord = dasheep->getCoord();
  for(float y = base_coord.y - 10; y < base_coord.y + 10; y += 0.1) {
    for(float x = base_coord.x - 10; x < base_coord.x + 10; x += 0.1) {
      MapUtil::Vector3 ori = dasheep->getCoord();
      std::cout << "ori: " << ori.x << " " << ori.y << " " << ori.z << std::endl;
      MapUtil::Vector3 dest = MapUtil::makeVector3(x, y, ori.z);
      float real_cost = MapUtil::distance(ori, dest) * 10.F / dasheep->getMovementTimeModifier();
      float cost = MapUtil::round(dasheep->getRegion()->getMoveCost(dasheep, ori, dest));
      MapUtil::round(dasheep->getRegion()->move(dasheep, MapUtil::getOrientationToTarget(ori.x, ori.y, dest.x, dest.y), dest, cost, adventure->getWorld()));
      MapUtil::Vector3 ori_back = dasheep->getCoord();
      std::cout << "ori_back: " << ori_back.x << " " << ori_back.y << " " << ori_back.z << std::endl;
      // move back
      float back_real_cost = MapUtil::distance(ori, dest) * 10.F / dasheep->getMovementTimeModifier();
      float back_cost = MapUtil::round(dasheep->getRegion()->getMoveCost(dasheep, dest, ori));
      MapUtil::round(dasheep->getRegion()->move(dasheep, MapUtil::getOrientationToTarget(dest.x, dest.y, ori.x, ori.y), ori, back_cost, adventure->getWorld()));
      MapUtil::Vector3 final = dasheep->getCoord();
      std::cout << "final: " << final.x << " " << final.y << " " << final.z << std::endl;
    }
  }
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  std::cout << "Duration: " << elapsed_seconds.count() << "s" << std::endl;
  delete adventure;
}

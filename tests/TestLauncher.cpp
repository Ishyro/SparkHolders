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
#include "data/Stance.h"

#include "data/ways/Race.h"

#include "data/skills/Skill.h"

#include "data/Settings.h"

#include "util/FileOpener.h"
#include "util/Logger.h"

#include "communication/Socket.h"
#include "communication/ServerSocket.h"
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
  #ifdef LOG
    Logger::init_logger("test.log");
  #endif

  auto start = std::chrono::system_clock::now();
  Adventure * adventure = FileOpener::AdventureOpener(adventureFile, true);
  adventure->getWorld()->generateWorld();
  MathUtil::Vector3i chunk_id = BlocksChunk::getChunkId(MathUtil::Vector3i(50, 100, 1));
  for(std::pair<MathUtil::Vector3i, Block *> pair : adventure->getWorld()->getChunk(chunk_id)->getBlocks(chunk_id)) {
    std::cout << pair.first.x << " " << pair.first.y << " " << pair.first.z << std::endl;
    std::cout << pair.second->name << std::endl;
  }
  Character * dasheep = adventure->getWorld()->getCharacters().front();
  Character * dahuman;
  for(Character * c : adventure->getWorld()->getCharacters()) {
    if(c->getRace()->name == "TXT_HOMINIDAN") {
      dahuman = c;
      break;
    }
  }
  Skill * attack = dahuman->getAttack();
  Stance * stance = dahuman->getStance(ITEM_SHORT_SWORD);
  Skill * s0 = stance->getAttack(0);
  Skill * s1 = stance->getAttack(1);
  Skill * s2 = stance->getAttack(2);
  Skill * block = dahuman->getDefense();
  std::cout << block->name << std::endl;
  std::cout << attack->name << std::endl;

  MathUtil::Coords coords = dasheep->getWorldCoords();
  Time time = adventure->getTime();
  std::cout << time.to_string_clock_terra() << std::endl;
  std::cout << "latitude: " << coords.latitude.x << "° " << coords.latitude.y << "' " << coords.latitude.z << "'' longitude: " << coords.longitude.x << "° " << coords.longitude.y << "' " << coords.longitude.z << "''" << std::endl;
  for(int32_t tick = 0; tick < ticks; tick++) {
    Time time = adventure->getTime();
    std::cout << time.to_string_clock() << std::endl;
    std::cout << time.to_string_clock_terra() << std::endl;
    std::cout << MathUtil::getLight(coords, time) << std::endl;
    std::cout << adventure->getLight(dasheep->getCoord()) << std::endl;
    //
    adventure->applyIteration();
    SpeechManager::clear();
    adventure->actAllProjectiles();
    adventure->incrTick();
  }
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  std::cout << "Duration: " << elapsed_seconds.count() << "s" << std::endl;
  delete adventure;
}

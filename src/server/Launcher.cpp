#include "data/Action.h"
#include "data/Adventure.h"

#include "utils/FileOpener.h"

#include "server/Launcher.h"

// to remove after testing
#include <iostream>
#include <chrono>
#include <ctime>

int main(int argc, char ** argv) {

  if (argc < 2) {
    std::cout << "Expected: ./Launcher <adventureFile>" << std::endl;
    return EXIT_FAILURE;
  }

  std::string adventureFile = argv[1];

  Adventure * adventure = FileOpener::AdventureOpener(adventureFile);

  while(true) {
    auto start = std::chrono::system_clock::now();
    adventure->incrTick();
    adventure->applySoulBurn();
    // ask playerActions
    std::list<Action *> actionsNPCs = adventure->getNPCsActions();
    // receive playerActions
    adventure->actAllProjectiles();
    adventure->executeActions(actionsNPCs);
    actionsNPCs.clear();
    adventure->applyDayLight();
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Round duration: " << elapsed_seconds.count() << "s\n";
  }
}

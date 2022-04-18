//#include <barrier>
#include <thread>
#include <vector>
#include <unistd.h>

#include "data/Action.h"
#include "data/Adventure.h"
#include "data/World.h"

#include "utils/FileOpener.h"

#include "communication/Socket.h"
#include "communication/ServerSocket.h"
#include "communication/Server.h"

#include "server/Launcher.h"
#include "server/Link.h"

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
  adventure->applyDayLight();

  // std::vector<std::thread > threads = std::vector<std::thread >(adventure->maxPlayers);
  std::vector<Link *> links = std::vector<Link *>(adventure->maxPlayers);
  ServerSocket ss = ServerSocket(45678, adventure->maxPlayers, true);
  //std::barrier sync_point(adventure->maxPlayers, Server::receive);
  for(int i = 0; i < adventure->maxPlayers; i++) {
    links[i] = new Link(ss.accept(), adventure);
    //threads[i] = std::thread(communicate, sockets[i], sync_point, adventure);
  }

  while(true) {
    auto start = std::chrono::system_clock::now();
    for(int i = 0; i < adventure->maxPlayers; i++) {
      links[i]->sendMap();
    }
    adventure->applyDayLight();
    adventure->incrTick();
    adventure->applySoulBurn();
    std::list<Action *> actionsPlayers = std::list<Action *>();
    std::list<Action *> actionsNPCs = adventure->getNPCsActions();
    // receive playerActions
    for(int i = 0; i < adventure->maxPlayers; i++) {
      actionsPlayers.push_back(links[i]->receiveAction());
    }
    adventure->actAllProjectiles();
    adventure->executeActions(actionsPlayers);
    adventure->executeActions(actionsNPCs);
    adventure->incrDayLight();
    actionsPlayers.clear();
    actionsNPCs.clear();
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    // std::cout << "Round duration: " << elapsed_seconds.count() << "s\n";
  }
}

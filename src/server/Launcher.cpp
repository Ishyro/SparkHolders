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
#include "data/Character.h"

void startCommunication(Link ** link, ServerSocket ss, Adventure * adventure) {
  bool done = false;
  while(!done) {
    try {
      *link = new Link(ss.accept(), adventure);
      done = true;
    } catch (CloseException &e) {
      delete *link;
    }
  }
}

void relinkCommunication(std::vector<Link *> * links, ServerSocket ss, bool * noPlayers, Adventure * adventure) {
  while(!*noPlayers) {
    Socket newSocket = ss.accept();
    bool used = false;
    std::string playerName;
    try {
      newSocket.write(std::string("RECONNECT"));
      playerName = newSocket.read();
    } catch (CloseException &e) {
      continue;
    }
    for(int i = 0; i < adventure->maxPlayers; i++) {
      if((*links)[i]->isClosed() && (*links)[i]->getPlayer()->name == playerName) {
        (*links)[i]->changeSocket(newSocket);
        used = true;
        newSocket.write(std::string("OK"));
        break;
      }
    }
    if(!used) {
      newSocket.close();
    }
  }
}

int main(int argc, char ** argv) {

  if (argc < 2) {
    std::cout << "Expected: ./Launcher <adventureFile>" << std::endl;
    return EXIT_FAILURE;
  }

  std::string adventureFile = argv[1];

  Adventure * adventure = FileOpener::AdventureOpener(adventureFile);
  adventure->applyDayLight();

  std::vector<std::thread > threads = std::vector<std::thread >(adventure->maxPlayers);
  std::vector<Link *> links = std::vector<Link *>(adventure->maxPlayers);
  ServerSocket ss = ServerSocket(45678, adventure->maxPlayers, true);
  //std::barrier sync_point(adventure->maxPlayers, Server::receive);
  for(int i = 0; i < adventure->maxPlayers; i++) {
    threads[i] = std::thread(startCommunication, &links[i], ss, adventure);
  }
  for(int i = 0; i < adventure->maxPlayers; i++) {
    threads[i].join();
  }
  threads.clear();
  bool noPlayers = false;
  std::thread thread = std::thread(relinkCommunication, &links, ss, &noPlayers, adventure);
  while(!noPlayers) {
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
      if(!links[i]->getPlayer()->isInWeakState()) {
        actionsPlayers.push_back(links[i]->receiveAction());
      }
    }
    adventure->actAllProjectiles();
    adventure->executeActions(actionsPlayers);
    adventure->executeActions(actionsNPCs);
    if(adventure->getTick() % 5 == 0) {
      adventure->incrDayLight();
    }
    actionsPlayers.clear();
    actionsNPCs.clear();
    noPlayers = true;
    for(int i = 0; i < adventure->maxPlayers; i++) {
      if(!links[i]->isClosed()) {
        noPlayers = false;
        break;
      }
    }
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    // std::cout << "Round duration: " << elapsed_seconds.count() << "s\n";
  }
  delete adventure;
  ss.close();
}

//#include <barrier>
#include <thread>
#include <vector>
#include <unistd.h>
#include <string>

#include "data/Action.h"
#include "data/Adventure.h"
#include "data/Character.h"
#include "data/World.h"

#include "data/Settings.h"

#include "utils/FileOpener.h"

#include "communication/Socket.h"
#include "communication/ServerSocket.h"
#include "communication/Server.h"
#include "communication/SpeechManager.h"

#include "server/Launcher.h"
#include "server/Link.h"

// to remove after testing
#include <iostream>
#include <chrono>
#include <ctime>

void startCommunication(Link ** link, ServerSocket ss, Adventure * adventure) {
  bool done = false;
  while(!done) {
    try {
      *link = new Link(ss.accept(), adventure);
      done = true;
    } catch (CloseException &e) {
      delete *link;
      *link = nullptr;
    }
  }
}

void relinkCommunication(std::vector<Link *> * links, ServerSocket ss, Adventure * adventure, int playersNumber) {
  while(true) {
    Socket newSocket = ss.accept();
    bool used = false;
    std::string playerName;
    try {
      newSocket.write(std::string("RECONNECT"));
      playerName = newSocket.read();
    } catch (CloseException &e) {
      continue;
    }
    for(int i = 0; i < playersNumber; i++) {
      if((*links)[i]->isClosed() && (*links)[i]->getPlayer()->name == playerName) {
        (*links)[i]->changeSocket(newSocket);
        used = true;
        try {
          newSocket.write(std::string("OK"));
          Server::sendTraductionPaths(newSocket, adventure);
          break;
        } catch (CloseException &e) {
          used = false;
          (*links)[i]->markClosed();
        }
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

  int playersNumber = adventure->maxPlayers;

  if (argc >= 3) {
    playersNumber = std::min(playersNumber, stoi(std::string(argv[2])));
  }

  std::vector<std::thread > threads = std::vector<std::thread >(playersNumber);
  std::vector<Link *> links = std::vector<Link *>(playersNumber);
  ServerSocket ss = ServerSocket(Settings::getPort(), playersNumber, true);
  //std::barrier sync_point(adventure->maxPlayers, Server::receive);
  for(int i = 0; i < playersNumber; i++) {
    threads[i] = std::thread(startCommunication, &links[i], ss, adventure);
  }
  for(int i = 0; i < playersNumber; i++) {
    threads[i].join();
  }
  threads.clear();
  bool noPlayers = false;
  std::thread thread = std::thread(relinkCommunication, &links, ss, adventure, playersNumber);
  while(!noPlayers) {
    auto start = std::chrono::system_clock::now();
    adventure->applyRoundIteration();
    for(int i = 0; i < playersNumber; i++) {
      links[i]->sendMap();
    }
    SpeechManager::clear();
    std::list<Action *> actionsPlayers = std::list<Action *>();
    std::list<Action *> actions = adventure->getNPCsActions();
    actions.sort();
    // receive playerActions
    for(int i = 0; i < playersNumber; i++) {
      if(!links[i]->getPlayer()->isInWeakState()) {
        actionsPlayers.push_back(links[i]->receiveAction());
      }
    }
    actionsPlayers.sort();
    actions.merge(actionsPlayers);
    adventure->actAllProjectiles();
    adventure->executeActions(actions);
    actionsPlayers.clear();
    actions.clear();
    noPlayers = true;
    for(int i = 0; i < playersNumber; i++) {
      if(!links[i]->isClosed()) {
        noPlayers = false;
        break;
      }
    }
    adventure->incrRound();
    adventure->incrDayLight();
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    // std::cout << "Round duration: " << elapsed_seconds.count() << "s\n";
  }
  delete adventure;
  ss.close();
}

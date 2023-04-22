#ifdef _WIN32_WINNT
  #include <winsock2.h>
  #include <windows.h>
  #include <thread>
#endif

#include <thread>
#include <vector>
#include <unistd.h>
#include <string>

#include "data/Action.h"
#include "data/Adventure.h"
#include "data/Character.h"
#include "data/World.h"

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

// to remove after testing
#include <iostream>
#include <chrono>
#include <ctime>

// Windows is great
typedef struct StartCommunicationParameter {
  Link ** link;
  ServerSocket * ss;
  Adventure * adventure;
} StartCommunicationParameter;

// Windows is great
typedef struct RelinkCommunicationParameter {
  std::vector<Link *> * links;
  ServerSocket * ss;
  Adventure * adventure;
  int playersNumber;
} RelinkCommunicationParameter;

void startCommunication(void * param) {
  Link ** link = ((StartCommunicationParameter *) param)->link;
  ServerSocket * ss = ((StartCommunicationParameter *) param)->ss;
  Adventure * adventure = ((StartCommunicationParameter *) param)->adventure;
  bool done = false;
  while(!done) {
    try {
      *link = new Link(ss->accept(), adventure);
      done = true;
    } catch (CloseException &e) {
      delete *link;
      *link = nullptr;
    }
  }
}

void relinkCommunication(void * param) {
  std::vector<Link *> * links = ((RelinkCommunicationParameter *) param)->links;
  ServerSocket * ss = ((RelinkCommunicationParameter *) param)->ss;
  Adventure * adventure = ((RelinkCommunicationParameter *) param)->adventure;
  int playersNumber = ((RelinkCommunicationParameter *) param)->playersNumber;
  while(true) {
    Socket newSocket = ss->accept();
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
          Server::sendTranslationPaths(newSocket, adventure);
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
    std::cout << "Expected: ./Launcher <adventureFile.commands>" << std::endl;
    return EXIT_FAILURE;
  }
  
  setlocale(LC_ALL, "");
  std::string adventureFile = argv[1];
  srand(Settings::getSeed());

  Adventure * adventure = FileOpener::AdventureOpener(adventureFile);
  adventure->applyDayLight();

  int playersNumber = adventure->maxPlayers;

  if (argc >= 3) {
    playersNumber = std::min(playersNumber, stoi(std::string(argv[2])));
  }
  
  std::vector<StartCommunicationParameter *> params = std::vector<StartCommunicationParameter *>(playersNumber);
  #ifdef _WIN32_WINNT
    std::vector<HANDLE> threads = std::vector<HANDLE>(playersNumber);
  #else
    std::vector<std::thread> threads = std::vector<std::thread>(playersNumber);
  #endif
  std::vector<Link *> links = std::vector<Link *>(playersNumber);
  ServerSocket * ss = new ServerSocket(Settings::getPort(), playersNumber, false);
  for(int i = 0; i < playersNumber; i++) {
    params[i] = new StartCommunicationParameter();
    params[i]->link = &links[i];
    params[i]->ss = ss;
    params[i]->adventure = adventure;
    #ifdef _WIN32_WINNT
      threads[i] = (HANDLE) _beginthreadex(NULL, 0, (_beginthreadex_proc_type) startCommunication, (void *) params[i], 0, NULL);
    #else
      threads[i] = std::thread(startCommunication, (void *) params[i]);
    #endif
  }
  for(int i = 0; i < playersNumber; i++) {
    #ifdef _WIN32_WINNT
      WaitForSingleObject(threads[i], INFINITE);
    #else
      threads[i].join();
    #endif
  }
  threads.clear();
  bool noPlayers = false;
  RelinkCommunicationParameter * param = new RelinkCommunicationParameter();
  param->links = &links;
  param->ss = ss;
  param->adventure = adventure;
  param->playersNumber = playersNumber;
  #ifdef _WIN32_WINNT
    HANDLE thread = (HANDLE) _beginthreadex(NULL, 0, (_beginthreadex_proc_type) relinkCommunication, (void *) param, 0, NULL);
  #else
    std::thread thread = std::thread(relinkCommunication, (void *) param);
  #endif
  while(!noPlayers) {
    auto start = std::chrono::system_clock::now();
    adventure->applyIteration();
    for(int i = 0; i < playersNumber; i++) {
      links[i]->sendMap();
    }
    SpeechManager::clear();
    adventure->getNPCsActions();
    // receive playerActions
    std::list<Action *> actionsPlayers = std::list<Action *>();
    for(int i = 0; i < playersNumber; i++) {
      if(links[i]->getPlayer()->getNeedToUpdateActions() && !links[i]->getPlayer()->isInWeakState()) {
        actionsPlayers.push_back(links[i]->receiveAction());
        links[i]->getPlayer()->setNeedToUpdateActions(false);
      }
    }
    adventure->mergeActions(actionsPlayers);
    actionsPlayers.clear();
    adventure->executeActions();
    adventure->actAllProjectiles();
    noPlayers = true;
    for(int i = 0; i < playersNumber; i++) {
      if(!links[i]->isClosed()) {
        noPlayers = false;
        break;
      }
    }
    adventure->incrTick();
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    // std::cout << "Tick duration: " << elapsed_seconds.count() << "s" << std::endl;
  }
  delete adventure;
  ss->close();
  delete ss;
}

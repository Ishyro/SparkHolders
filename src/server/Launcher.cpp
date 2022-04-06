#include <thread>
#include <vector>

#include "data/Action.h"
#include "data/Adventure.h"

#include "utils/FileOpener.h"

#include "communication/Socket.h"
#include "communication/ServerSocket.h"

#include "server/Launcher.h"

// to remove after testing
#include <iostream>
#include <chrono>
#include <ctime>

void communicate(Socket s) {
  for(std::string msg; msg != "CLOSED"; msg = s.read()) {
    if (msg != "") {
      std::cout << msg << std::endl;
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

  std::vector<std::thread > threads = std::vector<std::thread >(adventure->maxPlayers);
  std::vector<Socket > sockets = std::vector<Socket >(adventure->maxPlayers);
  ServerSocket ss = ServerSocket(45678, adventure->maxPlayers);
  for(int i = 0; i < adventure->maxPlayers; i++) {
    sockets[i] = ss.accept();
    threads[i] = std::thread(communicate, sockets[i]);
  }

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

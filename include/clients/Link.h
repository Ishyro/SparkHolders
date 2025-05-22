#ifndef _LINK_CLIENT_H_
#define _LINK_CLIENT_H_

#include <vector>
#include <map>
#include <list>

#ifdef LOG
  #include <ostream>
  #include <fstream>
#endif

#include "clients/EnglishKeyHolder.h"

#include "Values.h"

class Link {
  public:
    #ifdef LOG
    Link(std::string ip, int64_t port) {
      s = new Socket("socket_client.log");
      try {
        // win socket init
        #ifdef _WIN32_WINNT
          WSADATA d;
          if (WSAStartup(MAKEWORD(2, 2), &d)) {
            std::cerr << "Failed to initialize the socket." << std::endl;
          }
        #endif
        s->connection(ip, port);
      }
      catch(CloseException &e) { throw e; }
    }
    #else
    Link(std::string ip, int64_t port) {
      s = new Socket();
      try {
        // win socket init
        #ifdef _WIN32_WINNT
          WSADATA d;
          if (WSAStartup(MAKEWORD(2, 2), &d)) {
            std::cerr << "Failed to initialize the socket." << std::endl;
          }
        #endif
        s->connection(ip, port);
      }
      catch(CloseException &e) { throw e; }
    }
    #endif
    void initialize(std::string password);
    void listen();
    void sendAction(int32_t type, void * arg1, void * arg2, int32_t mana_cost);
    void receiveState(std::string msg);
    void sendReady();
    void sendPause();
    void sendUnpause();
    int64_t getTickRate();
    std::vector<Attributes *> getStartingAttributes();
    std::vector<Way *> getStartingWays();
    std::list<std::pair<const std::string, const std::string>> getWaysIncompatibilities();
    Character * getPlayer();
    Adventure * getAdventure();
    bool hasState();
    StateDisplay * getState();
    std::string getEnglishFromKey(std::string key);
    bool isClosed();
    bool isReady();
    bool isStarted();
    void close(bool shutdown);
    void receiveAdventure(std::string msg);
    std::string writeAction(int32_t type, void * arg1 = nullptr, void * arg2 = nullptr, int32_t mana_cost = 1);
    std::string writeBaseAction(int32_t type);
    std::string writeGearAction(int32_t type, ItemSlot * slot1, ItemSlot * slot2);
    std::string writeOrientedAction(int32_t type, MathUtil::Vector3 orientation);
    std::string writeTargetedAction(int32_t type, MathUtil::Target * target);
    std::string writeSkillAction(int32_t type, MathUtil::Target * target, Skill * skill, int32_t mana_cost);
    void sendChoices(std::string name, std::string attributes, std::string race, std::string origin, std::string culture, std::string religion, std::string profession);
  private:
    std::vector<Attributes *> * attributes;
    std::vector<Way *> * ways;
    std::list<std::pair<const std::string, const std::string>> * waysIncompatibilities;
    Character * character = nullptr;
    std::list<StateDisplay *> state_pile = std::list<StateDisplay *>();
    Adventure * adventure;
    bool ready = false;
    bool master;
    bool closed = false;
    bool started = false;
    int64_t tickrate;
    Socket * s;
    EnglishKeyHolder * key_holder;
    std::string language;
    #ifdef LOG
      std::ofstream log;
    #endif
};

#endif // _LINK_CLIENT_H_

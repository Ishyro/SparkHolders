#ifndef _LINK_CLIENT_H_
#define _LINK_CLIENT_H_

#include <vector>
#include <map>
#include <list>

#include "clients/Translator.h"

#include "Values.h"

class Link {
  public:
    Link(Socket s, std::string language):s(s),language(language) {}
    void initialize(std::string username, std::string password);
    void listen();
    void sendChoices(std::string name, std::string attributes, std::string race, std::string origin, std::string culture, std::string religion, std::string profession);
    void sendActions(
      std::vector<long> ids,
      std::vector<std::vector<int>> types,
      std::vector<std::vector<void *>> args1,
      std::vector<std::vector<void *>> args2,
      std::vector<std::vector<int>> overcharge_powers,
      std::vector<std::vector<int>> overcharge_durations,
      std::vector<std::vector<int>> overcharge_ranges
    );
    void receiveState(std::string msg);
    void sendReady();
    std::vector<Attributes *> getStartingAttributes();
    std::vector<Way *> getStartingWays();
    std::list<std::pair<const std::string, const std::string>> getWaysIncompatibilities();
    Character * getPlayer(long id);
    std::list<long> getPlayersId();
    std::list<Character *> getPlayers();
    Adventure * getAdventure();
    bool hasState();
    StateDisplay * getState();
    Translator * getTranslator();
    void changeLanguage(std::string language);
    bool isClosed();
    bool isReady();
    bool isStarted();
    void markClosed();
  private:
    std::vector<Attributes *> * attributes;
    std::vector<Way *> * ways;
    std::list<std::pair<const std::string, const std::string>> * waysIncompatibilities;
    std::map<const long, Character *> characters = std::map<const long, Character *>();
    std::list<StateDisplay *> state_pile = std::list<StateDisplay *>();
    Adventure * adventure;
    bool ready = false;
    bool master;
    bool closed = false;
    bool started = false;
    Socket s;
    Translator * t;
    std::string language;
};

#endif // _LINK_CLIENT_H_

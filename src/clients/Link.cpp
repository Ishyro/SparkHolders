#include "data/actions/Action.h"
#include "data/Adventure.h"
#include "data/Character.h"
#include "data/Map.h"
#include "data/World.h"

#include "communication/Socket.h"

#include "clients/Link.h"

#include "communication/Client.h"


Translator * Link::initialize(std::string language) {
  try {
    adventure = Client::receiveAdventure(s);
  } catch (const CloseException &e) {
    throw e;
  }
  return new Translator(adventure->getDatabase()->getTranslationPaths(), language);
}

void Link::sendChoices(std::string name, std::string attributes, std::string race, std::string origin, std::string culture, std::string religion, std::string profession) {
  try {
    Character * new_player = Client::sendChoices(s, adventure, name, attributes, race, origin, culture, religion, profession);
    players.insert(std::make_pair(new_player->id, new_player));
  } catch (const CloseException &e) {
    throw e;
  }
}

StateDisplay * Link::receiveState() {
  Character * player = nullptr;
  for(auto pair : players) {
    player = pair.second;
  }
  try {
    Character * new_player = nullptr;
    StateDisplay * game_state = Client::receiveState(s, adventure, new_player, need_to_update_actions);
    if(new_player != nullptr) {
      players.erase(new_player->id);
      players.insert(std::make_pair(new_player->id, new_player));
    }
    for(CharacterDisplay * display : game_state->characters) {
      if(player->id == display->id) {
        player->move(display->x, display->y, display->z, display->orientation, adventure->getWorld());
        player->setHp(display->hp);
        player->setMana(display->mana);
        player->setStamina(display->stamina);
        player->setSatiety(display->satiety);
        player->setCurrentSoulBurn(display->soulBurn);
        player->gainXP(display->xp - player->getXP());
      }
    }
    game_state->map = new Map(adventure->getWorld()->getMap(player->getX(), player->getY(), player->getZ()), player, adventure->getDatabase(), adventure->getWorld());
    player->setCurrentMap(game_state->map);
    return game_state;
  } catch (const CloseException &e) {
    throw e;
  }
}

void Link::sendAction(int type, void * arg1 = nullptr, void * arg2 = nullptr, int overcharge_power = 1, int overcharge_duration = 1, int overcharge_range = 1) {
  try {
    switch(type) {
      case ACTION_IDLE:
      case ACTION_RESPITE:
      case ACTION_REST:
      case ACTION_BREAKPOINT:
        Client::sendBaseAction(s, type);
        break;
      case ACTION_MOVE:
      case ACTION_STRIKE:
      case ACTION_HEAVY_STRIKE:
      case ACTION_SHOOT:
        Client::sendTargetedAction(s, type, (Target *) arg1);
        break;
      case ACTION_RELOAD:
      case ACTION_SWAP_GEAR:
      case ACTION_GRAB:
      case ACTION_USE_ITEM:
        Client::sendGearAction(s, type, (long) arg1);
        break;
      case ACTION_USE_SKILL:
        Client::sendSkillAction(s, type, (Target *) arg1, (Skill *) arg2, overcharge_power, overcharge_duration, overcharge_range);
        break;
      case ACTION_TALKING:
      case ACTION_ECONOMICS:
        // TODO
        break;
      default: ;
    }
  } catch (const CloseException &e) {
    throw e;
  }
}

std::vector<Attributes *> Link::getStartingAttributes() {
  std::list<Attributes *> l = adventure->getStartingAttributes();
  std::vector<Attributes *> v{ std::begin(l), std::end(l) };
  return v;
}

std::vector<Way *> Link::getStartingWays() {
  std::list<Way *> l = adventure->getStartingWays();
  std::vector<Way *> v{ std::begin(l), std::end(l) };
  return v;
}
std::list<std::pair<const std::string, const std::string>> Link::getWaysIncompatibilities() { return adventure->getDatabase()->getWaysIncompatibilities(); }

Character * Link::getPlayer(long id) { return players.at(id); }
Character * Link::getPlayer() {
  Character * player;
  for(auto pair : players) {
    player = pair.second;
  }
  return player;
}
Adventure * Link::getAdventure() { return adventure; }

bool Link::getNeedToUpdateActions() { return need_to_update_actions; }

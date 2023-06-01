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
    player = Client::sendChoices(s, adventure, name, attributes, race, origin, culture, religion, profession);
  } catch (const CloseException &e) {
    throw e;
  }
}

StateDisplay * Link::receiveState() {
  try {
    StateDisplay * game_state = Client::receiveState(s, adventure, &player, &need_to_update_actions);
    game_state->map = new Map(adventure->getWorld()->getMap(player->getCurrentMapId()), player, adventure->getDatabase());
    for(CharacterDisplay * display : game_state->characters) {
      if(player->id == display->id) {
        player->move(display->y, display->x, display->orientation, game_state->map_id);
        player->setHp(display->hp);
        player->setMana(display->mana);
        player->setStamina(display->stamina);
        player->setSatiety(display->satiety);
        player->setCurrentSoulBurn(display->soulBurn);
        player->gainXP(display->xp - player->getXP());
      }
    }
    return game_state;
  } catch (const CloseException &e) {
    throw e;
  }
}

void Link::sendAction(int type, void * arg1 = nullptr, void * arg2 = nullptr, int overcharge_power = 1, int overcharge_duration = 1, int overcharge_range = 1) {
  try {
    switch(type) {
      case IDLE:
      case RESPITE:
      case REST:
      case BREAKPOINT:
        Client::sendBaseAction(s, type);
        break;
      case MOVE:
      case STRIKE:
      case HEAVY_STRIKE:
      case SHOOT:
        Client::sendTargetedAction(s, type, (Target *) arg1);
        break;
      case RELOAD:
      case SWAP_GEAR:
      case GRAB:
      case USE_ITEM:
        Client::sendGearAction(s, type, (long) arg1);
        break;
      case USE_SKILL:
        Client::sendSkillAction(s, type, (Target *) arg1, (Skill *) arg2, overcharge_power, overcharge_duration, overcharge_range);
        break;
      case TALKING:
      case ECONOMICS:
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

Character * Link::getPlayer() { return player; }
Adventure * Link::getAdventure() { return adventure; }

bool Link::getNeedToUpdateActions() { return need_to_update_actions; }

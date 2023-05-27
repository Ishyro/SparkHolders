#include <sstream>

#include "data/Adventure.h"
#include "data/Attributes.h"
#include "data/Character.h"
#include "data/Gear.h"
#include "data/Map.h"
#include "data/Way.h"

#include "data/skills/Skill.h"

#include "communication/Socket.h"
#include "communication/Client.h"

#include "util/String.h"

namespace Client {

  Adventure * receiveAdventure(Socket s) {
    try {
      return FileOpener::AdventureOpener(s.read(), false);
    } catch (const CloseException &e) {
      throw e;
    }
  }

  StateDisplay * receiveState(Socket s, Adventure * adventure, Character ** player, bool * need_action) {
    try {
      std::string msg = s.read();
      std::stringstream * ss = new std::stringstream(msg);
      std::string player_str = String::extract(ss);
      if(player_str != "0") {
        (*player)->deepDelete();
        delete *player;
        *player = Character::full_from_string(player_str, adventure);
      }
      StateDisplay * result = adventure->update_state(String::extract(ss));
      *need_action = String::extract_bool(ss);
      delete ss;
      return result;
    } catch (const CloseException &e) {
      throw e;
    }
  }

  
  void sendBaseAction(Socket s, int type) {
    std::stringstream * ss = new std::stringstream();
    String::insert_int(ss, type);
    try {
      s.write(ss->str());
    } catch (const CloseException &e) {
      throw e;
    }
    delete ss;
  }

  void sendGearAction(Socket s, int type, GearPiece * piece) {
    std::stringstream * ss = new std::stringstream();
    String::insert_int(ss, type);
    String::insert(ss, Gear::piece_to_string(piece));
    try {
      s.write(ss->str());
    } catch (const CloseException &e) {
      throw e;
    }
    delete ss;
  }

  void sendTargetedAction(Socket s, int type, Target * target) {
    std::stringstream * ss = new std::stringstream();
    String::insert_int(ss, type);
    String::insert(ss, Map::target_to_string(target));
    try {
      s.write(ss->str());
    } catch (const CloseException &e) {
      throw e;
    }
    delete ss;
  }

  void sendSkillAction(Socket s, int type, Target * target, Skill * skill, int overcharge_power, int overcharge_duration, int overcharge_range) {
    std::stringstream * ss = new std::stringstream();
    String::insert_int(ss, type);
    String::insert(ss, Map::target_to_string(target));
    String::insert(ss, skill->name);
    String::insert_int(ss, overcharge_power);
    String::insert_int(ss, overcharge_duration);
    String::insert_int(ss, overcharge_range);
    try {
      s.write(ss->str());
    } catch (const CloseException &e) {
      throw e;
    }
    delete ss;
  }

  Character * sendChoices(Socket s, Adventure * adventure, std::string name, std::string attributes, std::string race, std::string origin, std::string culture, std::string religion, std::string profession) {
    std::stringstream * ss = new std::stringstream();
    String::insert(ss, name);
    String::insert(ss, attributes);
    String::insert(ss, race);
    String::insert(ss, origin);
    String::insert(ss, culture);
    String::insert(ss, religion);
    String::insert(ss, profession);
    try {
      s.write(ss->str());
      delete ss;
      return Character::full_from_string(s.read(), adventure);
    } catch (const CloseException &e) {
      throw e;
    }
  }
}

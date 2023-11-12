#include <sstream>

#include "data/Adventure.h"
#include "data/Attributes.h"
#include "data/Character.h"
#include "data/Map.h"
#include "data/Way.h"

#include "data/skills/Skill.h"

#include "communication/Socket.h"
#include "communication/Client.h"

#include "util/String.h"

namespace Client {

  Adventure * receiveAdventure(std::string msg, bool & master) {
    std::stringstream * ss = new std::stringstream(msg);
    // ignore socket_msg_type
    String::extract(ss);
    Adventure * result = FileOpener::AdventureOpener(String::extract(ss), false);
    master = String::extract_bool(ss);
    return result;
  }

  StateDisplay * receiveState(std::string msg, Adventure * adventure) {
    std::stringstream * ss = new std::stringstream(msg);
    // ignore socket_msg_type
    String::extract(ss);
    StateDisplay * result = adventure->update_state(String::extract(ss));
    result->need_to_send_actions = String::extract_bool(ss);
    delete ss;
    return result;
  }

  std::string writeActions(
    long id,
    std::vector<int> types,
    std::vector<void *> args1,
    std::vector<void *> args2,
    std::vector<int> overcharge_powers,
    std::vector<int> overcharge_durations,
    std::vector<int> overcharge_ranges
  ) {
    std::stringstream * ss = new std::stringstream();
    String::insert_long(ss, id);
    for(int i = 0; i < (int) types.size(); i++) {
      String::insert(ss, writeAction(types[i], args1[i], args2[i], overcharge_powers[i], overcharge_durations[i], overcharge_ranges[i]));
    }
    std::string result = ss->str();
    delete ss;
    return result;
  }

  std::string writeAction(int type, void * arg1, void * arg2, int overcharge_power, int overcharge_duration, int overcharge_range) {
    switch(type) {
      case ACTION_IDLE:
      case ACTION_RESPITE:
      case ACTION_REST:
      case ACTION_BREAKPOINT:
        return writeBaseAction(type);
        break;
      case ACTION_MOVE:
      case ACTION_STRIKE:
      case ACTION_HEAVY_STRIKE:
      case ACTION_SHOOT:
      case ACTION_ACTIVATION:
        return writeTargetedAction(type, (Target *) arg1);
        break;
      case ACTION_RELOAD:
      case ACTION_SWAP_GEAR:
      case ACTION_GRAB:
      case ACTION_USE_ITEM:
        return writeGearAction(type, (ItemSlot *) arg1, (ItemSlot *) arg2);
        break;
      case ACTION_USE_SKILL:
        return writeSkillAction(type, (Target *) arg1, (Skill *) arg2, overcharge_power, overcharge_duration, overcharge_range);
        break;
      case ACTION_TALKING:
      case ACTION_ECONOMICS:
        // TODO
        break;
      default: ;
    }
    return "";
  }
  
  std::string writeBaseAction(int type) {
    std::stringstream * ss = new std::stringstream();
    String::insert_int(ss, type);
    std::string result = ss->str();
    delete ss;
    return result;
  }

  std::string writeGearAction(int type, ItemSlot * slot1, ItemSlot * slot2) {
    std::stringstream * ss = new std::stringstream();
    String::insert_int(ss, type);
    String::insert_int(ss, slot1->x);
    String::insert_int(ss, slot1->y);
    String::insert_int(ss, slot1->slot);
    if(slot2 != nullptr) {
      String::insert_int(ss, slot2->x);
      String::insert_int(ss, slot2->y);
      String::insert_int(ss, slot2->slot);
    }
    std::string result = ss->str();
    delete ss;
    return result;
  }

  std::string writeTargetedAction(int type, Target * target) {
    std::stringstream * ss = new std::stringstream();
    String::insert_int(ss, type);
    String::insert(ss, Map::target_to_string(target));
    std::string result = ss->str();
    delete ss;
    return result;
  }

  std::string writeSkillAction(int type, Target * target, Skill * skill, int overcharge_power, int overcharge_duration, int overcharge_range) {
    std::stringstream * ss = new std::stringstream();
    String::insert_int(ss, type);
    String::insert(ss, Map::target_to_string(target));
    String::insert(ss, skill->name);
    String::insert_int(ss, overcharge_power);
    String::insert_int(ss, overcharge_duration);
    String::insert_int(ss, overcharge_range);
    std::string result = ss->str();
    delete ss;
    return result;
  }

  void sendChoices(Socket s, Adventure * adventure, std::string name, std::string attributes, std::string race, std::string origin, std::string culture, std::string religion, std::string profession) {
    std::stringstream * ss = new std::stringstream();
    String::insert_int(ss, SOCKET_MSG_CHOICE);
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
    } catch (const CloseException &e) {
      throw e;
    }
  }
}

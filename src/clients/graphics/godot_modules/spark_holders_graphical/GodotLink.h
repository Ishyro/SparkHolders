#ifndef _GODOT_LINK_H_
#define _GODOT_LINK_H_

#ifdef _WIN32_WINNT
  #include <winsock2.h>
  #include <windows.h>
  #include <thread>
#endif

#include <thread>

#include "core/object/ref_counted.h"

#include "data/Adventure.h"

#include "communication/Socket.h"

#include "clients/Link.h"

#include "Values.h"

#include <iostream>

#ifdef LOG
  #include <ostream>
  #include <fstream>
#endif

class GodotLink : public RefCounted {
  GDCLASS(GodotLink, RefCounted);
public:
  GodotLink():
    link(nullptr),
    state(nullptr)
  {
    #ifdef LOG
      log.open("log.txt");
    #endif
  }
  void initialize(String ip, int64_t port, String password);
  int64_t getTickRate();
  String getIncompatible(String tocheck, String attributes, String race, String origin, String culture, String religion, String profession);
  String getEnglishFromKey(String key);
  void sendChoices(String character, String attributes, String race, String origin, String culture, String religion, String profession);
  bool hasState();
  bool getState();
  Dictionary getMacros();
  String getTime();
  String getClock(bool terran_day);
  int64_t getLight();
  int64_t getBaseLight();
  int64_t getMaxLight();
  float getOrientationToTarget(Vector2 a, Vector2 b);
  Array getAvaillableBlocks();
  Dictionary getBlocks(int64_t sizeZ, int64_t radius);
  int64_t getPlayerId();
  Array getLights();
  Array getControlledParty();
  Dictionary getCharacters();
  Dictionary getProjectiles();
  Dictionary getFurnitures();
  Array getUpdatedFurnitures();
  String getRelation(String team1, String team2);
  Dictionary getDataFromItem(Item * item);
  Dictionary getDataFromBlock(String tile_name);
  Dictionary getDataFromClass(String class_name);
  Dictionary getDataFromRace(String race_name);
  Dictionary getDataFromWay(String way_name);
  Dictionary getDataFromCharacter(CharacterDisplay * character);
  Dictionary getStatsFromCharacter();
  Dictionary getInventoryFromCharacter();
  Dictionary getDataFromSkill(String skill_name);
  Array getSkillsFromCharacter();
  Dictionary getDataFromProjectile(ProjectileDisplay * projectile);
  Dictionary getDataFromFurniture(Furniture * furniture);
  Dictionary getItemSlot(int64_t item_id);
  Array getStartingAttributes();
  Array getStartingWays();
  void send_action(Dictionary action);
  void pause();
  void unpause();
  void close(bool shutdown);

protected:
  static void _bind_methods();
private:
  #ifdef LOG
    std::ofstream log;
  #endif
  Socket s;
  Link * link;
  StateDisplay * state;
  #ifdef _WIN32_WINNT
    HANDLE thread;
  #else
    std::thread thread;
  #endif
};

#endif // _GODOT_LINK_H_

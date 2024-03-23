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
#include "clients/Translator.h"

#include "Values.h"

#include <iostream>
#include <fstream>

class GodotLink : public RefCounted {
  GDCLASS(GodotLink, RefCounted);
public:
  GodotLink():
    link(nullptr),
    translator(nullptr),
    state(nullptr)
  {
    log.open("log.txt");
  }
  void initialize(String ip);
  bool hasState();
  bool getState();
  float getMoveCost(int64_t character_id, Vector3 ori, Vector3 dest);
  float getOrientationToTarget(Vector2 a, Vector2 b);
  Array getAvaillableBlocks();
  Dictionary getBlocks(int64_t character_id);
  Array getLights(int64_t character_id);
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
  Dictionary getStatsFromCharacter(int64_t character_id);
  Dictionary getInventoryFromCharacter(int64_t character_id);
  Dictionary getSkillsFromCharacter(int64_t character_id);
  Dictionary getDataFromProjectile(ProjectileDisplay * projectile);
  Dictionary getDataFromFurniture(Furniture * furniture);
  void send_actions(Dictionary actions);
  void close();

protected:
  static void _bind_methods();
private:
  Socket s;
  Link * link;
  Translator * translator;
  StateDisplay * state;
  std::ofstream log;
  std::map<long, int> mist_nbs = std::map<long, int>();
  #ifdef _WIN32_WINNT
    HANDLE thread;
  #else
    std::thread thread;
  #endif
};

#endif // _GODOT_LINK_H_

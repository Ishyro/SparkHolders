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
  float getMoveCost(int64_t character_id, float oriX, float oriY, float destX, float destY);
  float getOrientationToTarget(Vector2 a, Vector2 b);
  bool needTilesUpdate(int64_t character_id);
  Vector3 getSizes(int64_t character_id);
  Vector3 getOffsets(int64_t character_id);
  Array getAvaillableTiles();
  Array getTiles(int64_t character_id);
  Array getLights(int64_t character_id);
  Array getControlledParty();
  Dictionary getCharacters();
  Dictionary getProjectiles();
  Dictionary getFurnitures();
  String getRelation(String team1, String team2);
  Dictionary getCurrentRegions();
  int64_t getMapFromCoords(Vector3 coords);
  Dictionary getDataFromTile(String tile_name);
  Dictionary getDataFromCharacter(CharacterDisplay * character);
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

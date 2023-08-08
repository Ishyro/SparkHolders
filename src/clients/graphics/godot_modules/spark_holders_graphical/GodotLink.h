#ifndef _GODOT_LINK_H_
#define _GODOT_LINK_H_

#include "core/object/ref_counted.h"

#include "data/Adventure.h"

#include "communication/Socket.h"

#include "clients/Link.h"
#include "clients/Translator.h"

#include "Values.h"

class GodotLink : public RefCounted {
  GDCLASS(GodotLink, RefCounted);
public:
  GodotLink():
    link(nullptr),
    translator(nullptr),
    state(nullptr)
  {}
  void initialize(String ip);
  void receiveState();
  float getMoveCost(int64_t character_id, float oriX, float oriY, float destX, float destY);
  float getOrientationToTarget(Vector2 a, Vector2 b);
  Vector3 getSizes();
  Vector3 getOffsets();
  Array getAvaillableTiles();
  Array getTiles();
  Array getLights();
  Array getControlledParty();
  Dictionary getCharacters();
  Dictionary getProjectiles();
  String getRelation(String team1, String team2);
  Dictionary getDataFromTile(String tile_name);
  Dictionary getDataFromCharacter(long id);
  Dictionary getDataFromProjectile(long id);
  void close();

protected:
  static void _bind_methods();
private:
  Socket s;
  Link * link;
  Translator * translator;
  StateDisplay * state;
};

#endif // _GODOT_LINK_H_

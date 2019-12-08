#ifndef _TILE_H_
#define _TILE_H_

#include <string>

#include "src/data/Effect.h"

namespace tile {
  static long id_cpt = 0;
}

class Tile {
  public:
    const long id = ++tile::id_cpt; // fromage
    const std::string name;
    const int type;
    const bool breakable;
    const bool untraversable;
    Tile(std::string name) {
      // TODO XML_READER
    }
    Tile * damage(int value);
    int getHp();
    Tile * crumble();
  private:
    int hp;
};

#endif // _TILE_H_

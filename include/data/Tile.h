#ifndef _TILE_H_
#define _TILE_H_

#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>

#include "data/Effect.h"

#include "utils/FileOpener.h"

namespace tile {
  static long id_cpt = 0;
}

class Tile {
  public:
    const std::string name;
    const bool untraversable;
    const int light;
    Tile(
      std::string name,
      bool untraversable,
      int light
    ):
      name(name),
      untraversable(untraversable),
      light(light)
    {}
  private:
    const long id = ++tile::id_cpt;
};

#endif // _TILE_H_

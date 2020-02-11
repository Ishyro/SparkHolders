#ifndef _TILE_H_
#define _TILE_H_

#include <string>

#include "src/data/Effect.h"

namespace tile { 
  static long id_cpt = 0;
}

class Tile { 
  public:
    const std::string name;
    const bool untraversable;
    Tile(std::string name) { 
      // TODO XML_READER
    }
};

#endif // _TILE_H_

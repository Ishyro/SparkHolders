#ifndef _TILE_H_
#define _TILE_H_

#include <string>

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
    bool operator == (const Tile& t) const { return name == t.name; }
    bool operator != (const Tile& t) const { return !operator==(t); }
};

#endif // _TILE_H_

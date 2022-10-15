#ifndef _TILE_H_
#define _TILE_H_

#include <string>

class Tile {
  public:
    const std::string name;
    const bool untraversable;
    const bool opaque;
    const bool solid;
    const int light;
    Tile(
      std::string name,
      bool untraversable,
      bool opaque,
      bool solid,
      int light
    ):
      name(name),
      untraversable(untraversable),
      opaque(opaque),
      solid(solid),
      light(light)
    {}
    std::string to_string();
    static Tile * from_string(std::string to_read);
    bool operator == (const Tile& t) const { return name == t.name; }
    bool operator != (const Tile& t) const { return !operator==(t); }
};

#endif // _TILE_H_

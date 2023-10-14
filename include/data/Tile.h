#ifndef _TILE_H_
#define _TILE_H_

#include <string>

class Tile {
  public:
    const std::string name;
    const bool unwalkable;
    const bool opaque;
    const bool solid;
    const int light;
    const float ap_cost;
    Tile(
      std::string name,
      bool unwalkable,
      bool opaque,
      bool solid,
      int light,
      float ap_cost
    ):
      name(name),
      unwalkable(unwalkable),
      opaque(opaque),
      solid(solid),
      light(light),
      ap_cost(ap_cost)
    {}
    std::string to_string();
    static Tile * from_string(std::string to_read);
    bool operator == (const Tile& t) const { return name == t.name; }
    bool operator != (const Tile& t) const { return !operator==(t); }
};

#endif // _TILE_H_

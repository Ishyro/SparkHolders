#ifndef _TILE_H_
#define _TILE_H_

#include <string>

class Block {
  public:
    const std::string name;
    const bool unwalkable;
    const bool opaque;
    const bool allow_vertical;
    const int light;
    const float ap_cost;
    Block(
      std::string name,
      bool unwalkable,
      bool opaque,
      bool allow_vertical,
      int light,
      float ap_cost
    ):
      name(name),
      unwalkable(unwalkable),
      opaque(opaque),
      allow_vertical(allow_vertical),
      light(light),
      ap_cost(ap_cost)
    {}
    std::string to_string();
    static Block * from_string(std::string to_read);
    bool operator == (const Block& t) const { return name == t.name; }
    bool operator != (const Block& t) const { return !operator==(t); }
};

#endif // _TILE_H_

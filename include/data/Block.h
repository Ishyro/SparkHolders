#ifndef _TILE_H_
#define _TILE_H_

#include <string>

class Block {
  public:
    const std::string name;
    const int type;
    const std::string material;
    const bool unwalkable;
    const bool opaque;
    const int light;
    const int orientation;
    const float ap_cost;
    Block(
      std::string name,
      int type,
      std::string material,
      bool unwalkable,
      bool opaque,
      int light,
      int orientation,
      float ap_cost
    ):
      name(name),
      type(type),
      material(material),
      unwalkable(unwalkable),
      opaque(opaque),
      light(light),
      orientation(orientation),
      ap_cost(ap_cost)
    {}
    std::string to_string();
    static Block * from_string(std::string to_read);
    bool operator == (const Block& t) const { return name == t.name; }
    bool operator != (const Block& t) const { return !operator==(t); }
};

#endif // _TILE_H_

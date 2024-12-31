#ifndef _TILE_H_
#define _TILE_H_

#include <string>

#include "Values.h"

class Block {
  public:
    const std::string name;
    const int32_t type;
    const std::string material;
    const bool unwalkable;
    const bool opaque;
    const int32_t light;
    const int32_t orientation;
    const float ap_cost;
    Block(
      std::string name,
      int32_t type,
      std::string material,
      bool unwalkable,
      bool opaque,
      int32_t light,
      int32_t orientation,
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

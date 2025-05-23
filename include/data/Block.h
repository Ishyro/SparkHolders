#ifndef _BLOCK_H_
#define _BLOCK_H_

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
    const int32_t orientation_z;
    const float speed;
    Block(
      std::string name,
      int32_t type,
      std::string material,
      bool unwalkable,
      bool opaque,
      int32_t light,
      int32_t orientation_z,
      float speed
    ):
      name(name),
      type(type),
      material(material),
      unwalkable(unwalkable),
      opaque(opaque),
      light(light),
      orientation_z(orientation_z),
      speed(speed)
    {}
    std::string to_string();
    static Block * from_string(std::string to_read);
    bool operator == (const Block& t) const { return name == t.name; }
    bool operator != (const Block& t) const { return !operator==(t); }
};

#endif // _BLOCK_H_

#include "src/data/Tile.h"

Tile * Tile::damage(int value) {
  hp -= value;
  if (hp <= 0) {
    return crumble();
  }
  else {
    return this;
  }
}

int Tile::getHp() {return hp;}

Tile * Tile::crumble() {
  switch(type) {
    case WOOD:
      return new Tile("grass");
    case STONE:
      return new Tile("stone_path");
    case METAL:
      return new Tile("alloy_plate");
    case ICE:
      return new Tile("shallow_water");
    case FIRESTONE:
      return new Tile("magma");
  }
}

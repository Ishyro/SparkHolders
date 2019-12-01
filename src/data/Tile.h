#include "src/loader/TileLoader.h"
#include "src/data/Effect.h"

class Tile {
  public:
    const string name;
    const boolean breakable;
    const boolean untraversable;
    Tile(string name);
    void damage(int damage);
  private:
    int hp;
};

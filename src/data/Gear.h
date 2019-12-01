#include "src/data/Item.h"

class Gear {
  public:
    Gear(Item head, Item left_arm, Item right_arm, Item legs, Item left_ring, Item right_ring);
    Item equip(Item new_item);
  private:
    Item head;
    Item left_arm;
    Item right_arm;
    Item legs;
    Item body;
    Item left_ring;
    Item right_ring;
};

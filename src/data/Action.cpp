#include "data/Action.h"

void Action::execute(Adventure * adventure) {
  switch(this->type) {
    case MOVE:
      adventure->getWorld()->getMap(user->getCurrentMapId())->move(user, orientation);
      break;
    case CHANGE_MAP:
      if(user->getX() == link->x1 && user->getY() == link->y1 && user->getCurrentMapId() == link->map1->id) {
        user->move(link->x2, link->y2, link->orientation2);
        user->setCurrentMapId(link->map2->id);

      }
      else if(user->getX() == link->x2 && user->getY() == link->y2 && user->getCurrentMapId() == link->map2->id) {
        user->move(link->x1, link->y1, link->orientation1);
        user->setCurrentMapId(link->map1->id);
      }
      break;
    case GRAB:
      adventure->getWorld()->getMap(user->getCurrentMapId())->takeLoot(user);
      break;
    default: ;
  }
}

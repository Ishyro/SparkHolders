#include "src/data/Adventure.h"

Save * Adventure::save() {
  return new Save(this);
}

void Adventure::addPlayer(Character * player) {
  party.push_front(player);
}

void Adventure::removePlayer(Character * player) {
  party.remove(player);
}

bool Adventure::isWiped() {
  return party.empty();
}

int Adventure::getTick() {
  return world->getTick();
}

void Adventure::event() {
  int tick = world->getTick();
  for (auto e : events) {
    if (tick == e->tick) {
      e->activate(world);
    }
  }
}

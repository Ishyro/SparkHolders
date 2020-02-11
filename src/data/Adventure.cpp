#include "src/data/Adventure.h"

Save * Adventure::save() { 
  return new Save(this);
}

void Adventure::softMoveCharacterToMap(Character * character, long map_id, long x, long y) { 
  Map * map = world->getMap(map_id);
  long i = x;
  long j = y;
  bool conflict = false;
  int power = -1; // power = 0 in first loop turn
  int k = 0;
  while(conflict) { 
    if (k % 8 == 0) { 
      power++;
    }
    switch(k++ % 8 + 1) { // ORIENTATION
      case NORTH:
        j = y + power;
        break;
      case NORTH_EAST:
        j = y + power;
        i = x + power;
        break;
      case EAST:
        i = x + power;
        break;
      case SOUTH_EAST:
        j = y - power;
        i = x + power;
        break;
      case SOUTH:
        j = y - power;
        break;
      case SOUTH_WEST:
        j = y - power;
        i = x - power;
        break;
      case WEST:
        i = x - power;
        break;
      case NORTH_WEST:
        j = y + power;
        i = x - power;
        break;
    }
    if ((conflict = map->getTile(i, j)->untraversable)) { 
      continue;
    }
    for (auto c : map->getCharacters()) { 
      if (c->getX() == i && c->getY() == j) { 
        conflict = true;
        break;
      }
    }
  }
  world->getMap(character->getCurrentMapId())->removeCharacter(character);
  map->addCharacter(character);
  character->move(i, j);
  character->setCurrentMapId(map_id);
}

void Adventure::hardMoveCharacterToMap(Character * character, long map_id, long x, long y) { 
  Map * map = world->getMap(map_id);
  for (auto c : map->getCharacters()) { 
    if (c->getX() == x && c->getY() == y) { 
      map->killCharacter(c);
    }
  }
  // TODO TOCHECK will probably break because c should already be nullptr
  for (auto c : party) { 
    if (c->getX() == x && c->getY() == y) { 
      delete c;
      removePlayer(c);
    }
  }
  if (map->getTile(x, y)->untraversable) { 
    world->getMap(character->getCurrentMapId())->killCharacter(character);
    return;
  }
  world->getMap(character->getCurrentMapId())->removeCharacter(character);
  map->addCharacter(character);
  character->move(x, y);
  character->setCurrentMapId(map_id);
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

std::list<Character *> Adventure::getParty() { return party; }

std::list<Character *> Adventure::getPreservedPlayers() { return preserved_players; }

void Adventure::resurrect(Character * player, long map_id, long x, long y) { 
  if (std::find(preserved_players.begin(), preserved_players.end(), player) != preserved_players.end()) { 
    softMoveCharacterToMap(player, map_id, x, y);
  }
}

void Adventure::event() { 
  for (auto e : events) { 
    if (tick == e->tick) { 
      e->activate(this);
    }
  }
}

long Adventure::getTick() { return tick; }
void Adventure::incrTick() { tick++; }
World * Adventure::getWorld() { return world; }
void Adventure::addQuest(Quest * quest) { quests.push_front(quest); }
void Adventure::removeQuest(Quest * quest) { quests.remove(quest); }

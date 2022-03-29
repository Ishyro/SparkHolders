#include "data/Database.h"

Database::Database() {
  attributes = std::map<const std::string, Attributes * >();
  characters = std::map<const std::string, Character * >();
  effects = std::map<const std::string, Effect * >();
  events = std::map<const std::string, Event * >();
  items = std::map<const std::string, Item * >();
  projectiles = std::map<const std::string, Projectile * >();
  quests = std::map<const std::string, Quest * >();
  speechs = std::map<const std::string, Speech * >();
  tiles = std::map<const std::string, Tile * >();
  ways = std::map<const std::string, Way * >();
  weapons = std::map<const std::string, Weapon * >();
}

Attributes * Database::getAttributes(const std::string attributes) { return this->attributes.at(attributes); }
Character * Database::getCharacter(const std::string character) { return characters.at(character); }
Effect * Database::getEffect(const std::string effect) { return effects.at(effect); }
Event * Database::getEvent(const std::string event) { return events.at(event); }
Item * Database::getItem(const std::string item) { return items.at(item); }
Projectile * Database::getProjectile(const std::string projectile) { return projectiles.at(projectile); }
Quest * Database::getQuest(const std::string quest) { return quests.at(quest); }
Speech * Database::getSpeech(const std::string speech) { return speechs.at(speech); }
Tile * Database::getTile(const std::string tile) { return tiles.at(tile); }
Way * Database::getWay(const std::string way) { return ways.at(way); }
Weapon * Database::getWeapon(const std::string weapon) { return weapons.at(weapon); }

void Database::addAttributes(Attributes * attributes) { this->attributes.insert(std::make_pair(attributes->name, attributes)); }
void Database::addCharacter(Character * character) { characters.insert(std::make_pair(character->name, character)); }
void Database::addEffect(Effect * effect) { effects.insert(std::make_pair(effect->name, effect)); }
void Database::addEvent(Event * event) { events.insert(std::make_pair(event->name, event)); }
void Database::addItem(Item * item) { items.insert(std::make_pair(item->name, item)); }
void Database::addProjectile(Projectile * projectile) { projectiles.insert(std::make_pair(projectile->name, projectile)); }
void Database::addQuest(Quest * quest) { quests.insert(std::make_pair(quest->name, quest)); }
void Database::addSpeech(Speech * speech) { speechs.insert(std::make_pair(speech->name, speech)); }
void Database::addTile(Tile * tile) { tiles.insert(std::make_pair(tile->name, tile)); }
void Database::addWay(Way * way) { ways.insert(std::make_pair(way->name, way)); }
void Database::addWeapon(Weapon * weapon) { weapons.insert(std::make_pair(weapon->name, weapon)); }

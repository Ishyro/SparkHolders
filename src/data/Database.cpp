#include <stdexcept>

#include "data/Attributes.h"
#include "data/Character.h"
#include "data/Effect.h"
#include "data/Event.h"
#include "data/Item.h"
#include "data/Map.h"
#include "data/Projectile.h"
#include "data/Quest.h"
#include "data/skills/Skill.h"
#include "data/Speech.h"
#include "data/Tile.h"
#include "data/Way.h"
#include "data/Weapon.h"

#include "data/Database.h"

Database::Database() {
  macros = std::map<const std::string,const int>();

  attributes = std::map<const std::string, const Attributes * >();
  characters = std::map<const std::string, const Character * >();
  effects = std::map<const std::string, const Effect * >();
  events = std::map<const std::string, const Event * >();
  items = std::map<const std::string, const Item * >();
  gears = std::map<const std::string, const Gear * >();
  maps = std::map<const std::string, const Map * >();
  projectiles = std::map<const std::string, const Projectile * >();
  ammunition = std::map<const std::string, const Ammunition * >();
  quests = std::map<const std::string, const Quest * >();
  skills = std::map<const std::string, const Skill * >();
  pseudoSkills = std::map<const std::string, const PseudoSkill * >();
  speechs = std::map<const std::string, const Speech * >();
  tiles = std::map<const std::string, const Tile * >();
  ways = std::map<const std::string, const Way * >();
  weapons = std::map<const std::string, const Weapon * >();
  relations = std::map<const std::string, std::map<const std::string, int>>();
  waysIncompatibilities = std::list<std::pair<const std::string, const std::string>>();
  paths = std::list<std::string>();

  // effect_duration_type

  macros.insert(std::make_pair("INSTANT", INSTANT));
  macros.insert(std::make_pair("INFINITE", INFINITE));
  macros.insert(std::make_pair("TEMPORARY", TEMPORARY));

  // effect_type

  macros.insert(std::make_pair("HP", HP));
  macros.insert(std::make_pair("MANA", MANA));
  macros.insert(std::make_pair("HP_MAX", HP_MAX));
  macros.insert(std::make_pair("MANA_MAX", MANA_MAX));
  macros.insert(std::make_pair("STAMINA", STAMINA));
  macros.insert(std::make_pair("SATIETY", SATIETY));
  macros.insert(std::make_pair("ARMOR", ARMOR));
  macros.insert(std::make_pair("SOULBURNTRESHOLD", SOULBURNTRESHOLD));
  macros.insert(std::make_pair("FLOW", FLOW));
  macros.insert(std::make_pair("VISION_RANGE", VISION_RANGE));
  macros.insert(std::make_pair("VISION_POWER", VISION_POWER));
  macros.insert(std::make_pair("DETECTION_RANGE", DETECTION_RANGE));
  macros.insert(std::make_pair("EXPERIENCE", EXPERIENCE));
  macros.insert(std::make_pair("DAMAGE", DAMAGE));
  macros.insert(std::make_pair("DAMAGE_BUFF", DAMAGE_BUFF));
  macros.insert(std::make_pair("DAMAGE_REDUCTION", DAMAGE_REDUCTION));
  macros.insert(std::make_pair("DAMAGE_MULTIPLIER", DAMAGE_MULTIPLIER));
  macros.insert(std::make_pair("CHANNELING", CHANNELING));
  macros.insert(std::make_pair("STUNNED", STUNNED));
  macros.insert(std::make_pair("SLEEPING", SLEEPING));
  macros.insert(std::make_pair("FLY", FLY));
  macros.insert(std::make_pair("LIGHT", LIGHT));
  macros.insert(std::make_pair("CLOAKED", CLOAKED));
  macros.insert(std::make_pair("INVISIBLE", INVISIBLE));
  macros.insert(std::make_pair("ETHERAL", ETHERAL));
  macros.insert(std::make_pair("INVULNERABLE", INVULNERABLE));
  macros.insert(std::make_pair("ACTION_TIME_MODIFIER", ACTION_TIME_MODIFIER));
  macros.insert(std::make_pair("HAND_ACTION_TIME_MODIFIER", HAND_ACTION_TIME_MODIFIER));
  macros.insert(std::make_pair("SKILL_TIME_MODIFIER", SKILL_TIME_MODIFIER));
  macros.insert(std::make_pair("MOVEMENT_TIME_MODIFIER", MOVEMENT_TIME_MODIFIER));

  // weapon_type

  macros.insert(std::make_pair("LONG_SWORD", LONG_SWORD));
  macros.insert(std::make_pair("SHORT_SWORD", SHORT_SWORD));
  macros.insert(std::make_pair("CURVED_SWORD", CURVED_SWORD));
  macros.insert(std::make_pair("RAPIER", RAPIER));
  macros.insert(std::make_pair("SPEAR", SPEAR));
  macros.insert(std::make_pair("GLAIVE", GLAIVE));
  macros.insert(std::make_pair("AXE", AXE));
  macros.insert(std::make_pair("WARHAMMER", WARHAMMER));
  macros.insert(std::make_pair("DAGGER", DAGGER));
  macros.insert(std::make_pair("FIST", FIST));
  macros.insert(std::make_pair("BOW", BOW));
  macros.insert(std::make_pair("CROSSBOW", CROSSBOW));
  macros.insert(std::make_pair("MAGIC_STAFF", MAGIC_STAFF));
  macros.insert(std::make_pair("RIFLE", RIFLE));
  macros.insert(std::make_pair("PISTOL", PISTOL));
  macros.insert(std::make_pair("SLING", SLING));

  // item_type

  macros.insert(std::make_pair("UNEQUIPABLE", UNEQUIPABLE));
  macros.insert(std::make_pair("HEAD", HEAD));
  macros.insert(std::make_pair("ARMS", ARMS));
  macros.insert(std::make_pair("BODY", BODY));
  macros.insert(std::make_pair("LEGS", LEGS));
  macros.insert(std::make_pair("LANTERN", LANTERN));
  macros.insert(std::make_pair("FULL_BODY", FULL_BODY));
  macros.insert(std::make_pair("RING", RING));
  macros.insert(std::make_pair("AMULET", AMULET));

  macros.insert(std::make_pair("LEFT_RING", LEFT_RING));
  macros.insert(std::make_pair("RIGHT_RING", RIGHT_RING));

  // event_type

  macros.insert(std::make_pair("CALAMITY", CALAMITY));
  macros.insert(std::make_pair("BLESSING", BLESSING));

  // character_apparition_type

  macros.insert(std::make_pair("SOFT", SOFT));
  macros.insert(std::make_pair("HARD", HARD));

  // movement_type

  macros.insert(std::make_pair("FLYING", FLYING));
  macros.insert(std::make_pair("WALKING", WALKING));
  macros.insert(std::make_pair("TELEPORT", TELEPORT));

  // skill_target

  macros.insert(std::make_pair("SELF", SELF));
  macros.insert(std::make_pair("TARGET", TARGET));
  macros.insert(std::make_pair("TILE", TILE));

  // skill_type

  macros.insert(std::make_pair("CHANNELED_SKILL", CHANNELED_SKILL));
  macros.insert(std::make_pair("MAP_LINKER_SKILL", MAP_LINKER_SKILL));
  macros.insert(std::make_pair("MIND_CONTROL_SKILL", MIND_CONTROL_SKILL));
  macros.insert(std::make_pair("PROJECTILE_SKILL", PROJECTILE_SKILL));
  macros.insert(std::make_pair("RESURRECTION_SKILL", RESURRECTION_SKILL));
  macros.insert(std::make_pair("SIMPLE_SKILL", SIMPLE_SKILL));
  macros.insert(std::make_pair("SUMMON_SKILL", SUMMON_SKILL));
  macros.insert(std::make_pair("TEAM_CHANGER_SKILL", TEAM_CHANGER_SKILL));
  macros.insert(std::make_pair("TELEPORT_SKILL", TELEPORT_SKILL));
  macros.insert(std::make_pair("TILE_SWAP_SKILL", TILE_SWAP_SKILL));

  // skill_overcharge_type

  macros.insert(std::make_pair("NO_OVERCHARGE", NO_OVERCHARGE));
  macros.insert(std::make_pair("LINEAR", LINEAR));
  macros.insert(std::make_pair("SQUARE", SQUARE));
  macros.insert(std::make_pair("EXPONENTIAL", EXPONENTIAL));

  // map_link type

  macros.insert(std::make_pair("THROUGH", THROUGH));
  macros.insert(std::make_pair("BOUNCE", BOUNCE));
  macros.insert(std::make_pair("BACK", BACK));

  // projectile_type

  macros.insert(std::make_pair("FIRE_FORM", FIRE_FORM));
  macros.insert(std::make_pair("LIGHTNING_FORM", LIGHTNING_FORM));
  macros.insert(std::make_pair("AIR", AIR));
  macros.insert(std::make_pair("ARROW", ARROW));
  macros.insert(std::make_pair("BULLET", BULLET));
  macros.insert(std::make_pair("SPECIAL", SPECIAL));

  // character_type

  macros.insert(std::make_pair("HOMO", HOMO));
  macros.insert(std::make_pair("UNDEAD", UNDEAD));
  macros.insert(std::make_pair("NATURE", NATURE));
  macros.insert(std::make_pair("OMONCULUS", OMONCULUS));
  macros.insert(std::make_pair("WALL", WALL));

  // damage_type

  macros.insert(std::make_pair("DAMAGE_TYPE_NUMBER", DAMAGE_TYPE_NUMBER));

  macros.insert(std::make_pair("SLASH_DAMAGE", SLASH_DAMAGE));
  macros.insert(std::make_pair("PUNCTURE_DAMAGE", PUNCTURE_DAMAGE));
  macros.insert(std::make_pair("IMPACT_DAMAGE", IMPACT_DAMAGE));
  macros.insert(std::make_pair("FIRE_DAMAGE", FIRE_DAMAGE));
  macros.insert(std::make_pair("LIGHTNING_DAMAGE", LIGHTNING_DAMAGE));
  macros.insert(std::make_pair("COLD_DAMAGE", COLD_DAMAGE));
  macros.insert(std::make_pair("POISON_DAMAGE", POISON_DAMAGE));
  macros.insert(std::make_pair("NEUTRAL_DAMAGE", NEUTRAL_DAMAGE));
  macros.insert(std::make_pair("MIND_DAMAGE", MIND_DAMAGE));
  macros.insert(std::make_pair("TRUE_DAMAGE", TRUE_DAMAGE));
  macros.insert(std::make_pair("SOUL_DAMAGE", SOUL_DAMAGE));

  // loot_type

  macros.insert(std::make_pair("CORPSE", CORPSE));
  macros.insert(std::make_pair("TREASURE", TREASURE));

  // quest_step_type

  macros.insert(std::make_pair("SLAY", SLAY));
  macros.insert(std::make_pair("OBTAIN_ITEM", OBTAIN_ITEM));
  macros.insert(std::make_pair("OBTAIN_WEAPON", OBTAIN_WEAPON));
  macros.insert(std::make_pair("DISCOVER", DISCOVER));
  macros.insert(std::make_pair("TALK", TALK));

  // way_type

  macros.insert(std::make_pair("RACE", RACE));
  macros.insert(std::make_pair("ORIGIN", ORIGIN));
  macros.insert(std::make_pair("CULTURE", CULTURE));
  macros.insert(std::make_pair("RELIGION", RELIGION));
  macros.insert(std::make_pair("PROFESSION", PROFESSION));
  macros.insert(std::make_pair("TITLE", TITLE));

  // speech_type

  macros.insert(std::make_pair("MSG_TEXT", MSG_TEXT));
  macros.insert(std::make_pair("MSG_VOCAL", MSG_VOCAL));
  macros.insert(std::make_pair("MSG_LOG", MSG_LOG));
  macros.insert(std::make_pair("MSG_WARNING", MSG_WARNING));
  macros.insert(std::make_pair("MSG_ERROR", MSG_ERROR));

  // teams relations

  macros.insert(std::make_pair("SAME", SAME));
  macros.insert(std::make_pair("ALLY", ALLY));
  macros.insert(std::make_pair("NEUTRAL", NEUTRAL));
  macros.insert(std::make_pair("ENEMY", ENEMY));
}

const int Database::getTargetFromMacro(const std::string macro) { return macros.find(macro) != macros.end() ? macros.at(macro) : stoi(macro); }

const Attributes * Database::getAttributes(const std::string attributes) { return this->attributes.at(attributes); }
const Character * Database::getCharacter(const std::string character) { return characters.at(character); }
const Effect * Database::getEffect(const std::string effect) { return effects.at(effect); }
const Event * Database::getEvent(const std::string event) { return events.at(event); }
const Item * Database::getItem(const std::string item) { return items.at(item); }
const Gear * Database::getGear(const std::string gear) { return gears.at(gear); }
const Map * Database::getMap(const std::string map) { return maps.at(map); }
const Projectile * Database::getProjectile(const std::string projectile) { return projectiles.at(projectile); }
const Ammunition * Database::getAmmunition(const std::string ammo) { return ammunition.at(ammo); }
const Quest * Database::getQuest(const std::string quest) { return quests.at(quest); }
const Skill * Database::getSkill(const std::string skill) { return skills.at(skill); }
const PseudoSkill * Database::getPseudoSkill(const std::string pseudoSkill) { return pseudoSkills.at(pseudoSkill); }
const Speech * Database::getSpeech(const std::string speech) { return speechs.at(speech); }
const Tile * Database::getTile(const std::string tile) { return tiles.at(tile); }
const Way * Database::getWay(const std::string way) { return ways.at(way); }
const Weapon * Database::getWeapon(const std::string weapon) { return weapons.at(weapon); }
const int Database::getRelation(const std::string team1, const std::string team2) {
  if(team1 == team2) {
    return SAME;
  }
  try {
    return relations.at(team1).at(team2);
  } catch (const std::out_of_range &e) {
    return NEUTRAL;
  }
}

std::list<std::string> Database::getTranslationPaths() { return paths; }

std::list<std::pair<const std::string, const std::string>> Database::getWaysIncompatibilities() { return waysIncompatibilities; }

void Database::addAttributes(const Attributes * attributes) { this->attributes.insert(std::make_pair(attributes->name, attributes)); }
void Database::addCharacter(const Character * character) { characters.insert(std::make_pair(character->name, character)); }
void Database::addEffect(const Effect * effect) { effects.insert(std::make_pair(effect->name, effect)); }
void Database::addEvent(const Event * event) { events.insert(std::make_pair(event->name, event)); }
void Database::addItem(const Item * item) { items.insert(std::make_pair(item->name, item)); }
void Database::addGear(const Gear * gear) { gears.insert(std::make_pair(gear->name, gear)); }
void Database::addMap(const Map * map) { maps.insert(std::make_pair(map->name, map)); }
void Database::addProjectile(const Projectile * projectile) { projectiles.insert(std::make_pair(projectile->name, projectile)); }
void Database::addAmmunition(const Ammunition * ammo) { ammunition.insert(std::make_pair(ammo->projectile->name, ammo)); }
void Database::addQuest(const Quest * quest) { quests.insert(std::make_pair(quest->name, quest)); }
void Database::addSkill(const Skill * skill) { skills.insert(std::make_pair(skill->name, skill)); }
void Database::addPseudoSkill(const PseudoSkill * pseudoSkill) { pseudoSkills.insert(std::make_pair(pseudoSkill->name, pseudoSkill)); }
void Database::addSpeech(const Speech * speech) { speechs.insert(std::make_pair(speech->name, speech)); }
void Database::addTile(const Tile * tile) { tiles.insert(std::make_pair(tile->name, tile)); }
void Database::addWay(const Way * way) { ways.insert(std::make_pair(way->name, way)); }
void Database::addWeapon(const Weapon * weapon) { weapons.insert(std::make_pair(weapon->name, weapon)); }
void Database::addRelation(const std::string team1, const std::string team2, int relation) {
  if(relations.find(team1) == relations.end()) {
    std::map<const std::string, int> map = std::map<const std::string, int>();
    relations.insert(std::make_pair(team1, map));
  }
  relations.at(team1).insert(std::make_pair(team2, relation));
  if(relations.find(team2) == relations.end()) {
    std::map<const std::string, int> map = std::map<const std::string, int>();
    relations.insert(std::make_pair(team2, map));
  }
  relations.at(team2).insert(std::make_pair(team1, relation));
}

void Database::addWayImcompatibility(const std::string way1, const std::string way2) {
  waysIncompatibilities.push_back(std::make_pair(way1, way2));
}
void Database::addTranslationPath(const std::string path) { paths.push_back(path); };

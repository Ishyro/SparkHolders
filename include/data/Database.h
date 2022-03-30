#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <map>
#include <string>
#include <cmath>
#include <algorithm>

#include "Values.h"

#include "ai/AI.h"
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

class Database {
  public:
    Database();
    int getTargetFromMacro(const std::string macro);

    AI * getAI(const std::string ai);
    Attributes * getAttributes(const std::string attributes);
    Character * getCharacter(const std::string character);
    Effect * getEffect(const std::string effect);
    Event * getEvent(const std::string event);
    Item * getItem(const std::string item);
    Map * getMap(const std::string map);
    Projectile * getProjectile(const std::string projectile);
    Ammunition * getAmmunition(const std::string ammunition);
    Quest * getQuest(const std::string quest);
    Skill * getSkill(const std::string skill);
    Speech * getSpeech(const std::string speech);
    Tile * getTile(const std::string tile);
    Way * getWay(const std::string way);
    Weapon * getWeapon(const std::string weapon);

    void addAI(AI * ai);
    void addAttributes(Attributes * attributes);
    void addCharacter(Character * character);
    void addEffect(Effect * effect);
    void addEvent(Event * event);
    void addItem(Item * item);
    void addMap(Map * map);
    void addProjectile(Projectile * projectile);
    void addAmmunition(Ammunition * ammunition);
    void addQuest(Quest * quest);
    void addSkill(Skill * skill);
    void addSpeech(Speech * speech);
    void addTile(Tile * tile);
    void addWay(Way * way);
    void addWeapon(Weapon * weapon);

  private:
    std::map<const std::string, int> macros;

    std::map<const std::string, AI * > ais;
    std::map<const std::string, Attributes * > attributes;
    std::map<const std::string, Character * > characters;
    std::map<const std::string, Effect * > effects;
    std::map<const std::string, Event * > events;
    std::map<const std::string, Item * > items;
    std::map<const std::string, Map * > maps;
    std::map<const std::string, Projectile * > projectiles;
    std::map<const std::string, Ammunition * > ammunitions;
    std::map<const std::string, Quest * > quests;
    std::map<const std::string, Skill * > skills;
    std::map<const std::string, Speech * > speechs;
    std::map<const std::string, Tile * > tiles;
    std::map<const std::string, Way * > ways;
    std::map<const std::string, Weapon * > weapons;

};

#endif // _DATABASE_H_
